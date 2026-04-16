#pragma once
#include "const.h"
#include "hiredis.h"
#include <queue>
#include <atomic>
#include <mutex>
#include "Singleton.h"
#include <cstring>
#include <iostream>

class RedisConPool {
public:
    RedisConPool(size_t poolSize, const char* host, int port, const char* pwd)
        : poolSize_(poolSize), host_(host), port_(port), b_stop_(false), pwd_(pwd), counter_(0), fail_count_(0) {

        std::cout << "========== RedisConPool 构造函数开始 ==========" << std::endl;
        std::cout << "poolSize: " << poolSize << std::endl;
        std::cout << "host: " << (host ? host : "null") << std::endl;
        std::cout << "port: " << port << std::endl;
        std::cout << "pwd: " << (pwd ? pwd : "null") << std::endl;

        int success_count = 0;
        for (size_t i = 0; i < poolSize_; ++i) {
            std::cout << "\n--- 第 " << (i + 1) << " 次连接尝试 ---" << std::endl;

            // 连接 Redis
            auto* context = redisConnect(host, port);
            if (context == nullptr) {
                std::cout << "❌ redisConnect 返回 nullptr" << std::endl;
                continue;
            }

            if (context->err != 0) {
                std::cout << "❌ 连接错误: " << context->errstr << std::endl;
                redisFree(context);
                continue;
            }

            std::cout << "✓ TCP 连接成功" << std::endl;

            // 认证
            auto reply = (redisReply*)redisCommand(context, "AUTH %s", pwd);
            if (reply == nullptr) {
                std::cout << "❌ AUTH 命令返回 nullptr" << std::endl;
                redisFree(context);
                continue;
            }

            if (reply->type == REDIS_REPLY_ERROR) {
                std::cout << "❌ 认证失败: " << reply->str << std::endl;
                freeReplyObject(reply);
                redisFree(context);
                continue;
            }

            std::cout << "✓✓✓ 认证成功 ✓✓✓" << std::endl;
            freeReplyObject(reply);
            connections_.push(context);
            success_count++;
        }

        std::cout << "\n========== 初始化完成 ==========" << std::endl;
        std::cout << "成功连接数: " << success_count << " / " << poolSize_ << std::endl;
        std::cout << "连接队列大小: " << connections_.size() << std::endl;

        // 启动心跳检测线程
        check_thread_ = std::thread([this]() {
            while (!b_stop_) {
                counter_++;
                if (counter_ >= 60) {
                    checkThreadPro();
                    counter_ = 0;
                }
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
            });
    }

    ~RedisConPool() {
        std::cout << "RedisConPool 析构" << std::endl;
    }

    void ClearConnections() {
        std::lock_guard<std::mutex> lock(mutex_);
        while (!connections_.empty()) {
            auto* context = connections_.front();
            redisFree(context);
            connections_.pop();
        }
    }

    redisContext* getConnection() {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] {
            if (b_stop_) {
                return true;
            }
            return !connections_.empty();
            });
        if (b_stop_) {
            return nullptr;
        }
        auto* context = connections_.front();
        connections_.pop();
        return context;
    }

    redisContext* getConNonBlock() {
        std::unique_lock<std::mutex> lock(mutex_);
        if (b_stop_) {
            return nullptr;
        }
        if (connections_.empty()) {
            return nullptr;
        }
        auto* context = connections_.front();
        connections_.pop();
        return context;
    }

    void returnConnection(redisContext* context) {
        std::lock_guard<std::mutex> lock(mutex_);
        if (b_stop_) {
            return;
        }
        connections_.push(context);
        cond_.notify_one();
    }

    void Close() {
        b_stop_ = true;
        cond_.notify_all();
        if (check_thread_.joinable()) {
            check_thread_.join();
        }
    }

private:
    bool reconnect() {
        std::cout << "尝试重连 Redis..." << std::endl;
        auto context = redisConnect(host_, port_);
        if (context == nullptr || context->err != 0) {
            if (context != nullptr) {
                redisFree(context);
            }
            std::cout << "❌ 重连失败" << std::endl;
            return false;
        }

        auto reply = (redisReply*)redisCommand(context, "AUTH %s", pwd_);
        if (reply->type == REDIS_REPLY_ERROR) {
            std::cout << "❌ 重连认证失败" << std::endl;
            freeReplyObject(reply);
            redisFree(context);
            return false;
        }

        freeReplyObject(reply);
        std::cout << "✓ 重连认证成功" << std::endl;
        returnConnection(context);
        return true;
    }

    void checkThreadPro() {
        size_t pool_size;
        {
            std::lock_guard<std::mutex> lock(mutex_);
            pool_size = connections_.size();
        }

        for (int i = 0; i < pool_size && !b_stop_; ++i) {
            auto* context = getConNonBlock();
            if (context == nullptr) {
                break;
            }

            redisReply* reply = nullptr;
            try {
                reply = (redisReply*)redisCommand(context, "PING");
                if (context->err) {
                    std::cout << "Connection error: " << context->err << std::endl;
                    if (reply) freeReplyObject(reply);
                    redisFree(context);
                    fail_count_++;
                    continue;
                }

                if (!reply || reply->type == REDIS_REPLY_ERROR) {
                    std::cout << "PING failed" << std::endl;
                    if (reply) freeReplyObject(reply);
                    redisFree(context);
                    fail_count_++;
                    continue;
                }

                freeReplyObject(reply);
                returnConnection(context);
            }
            catch (std::exception& exp) {
                if (reply) freeReplyObject(reply);
                redisFree(context);
                fail_count_++;
            }
        }

        while (fail_count_ > 0) {
            auto res = reconnect();
            if (res) {
                fail_count_--;
            }
            else {
                break;
            }
        }
    }

    std::atomic<bool> b_stop_;
    size_t poolSize_;
    const char* host_;
    const char* pwd_;
    int port_;
    std::queue<redisContext*> connections_;
    std::atomic<int> fail_count_;
    std::mutex mutex_;
    std::condition_variable cond_;
    std::thread check_thread_;
    int counter_;
};

class RedisMgr : public Singleton<RedisMgr>,
    public std::enable_shared_from_this<RedisMgr>
{
    friend class Singleton<RedisMgr>;
public:
    ~RedisMgr() {
        std::cout << "RedisMgr 析构" << std::endl;
    }

    bool Get(const std::string& key, std::string& value);
    bool Set(const std::string& key, const std::string& value);
    bool LPush(const std::string& key, const std::string& value);
    bool LPop(const std::string& key, std::string& value);
    bool RPush(const std::string& key, const std::string& value);
    bool RPop(const std::string& key, std::string& value);
    bool HSet(const std::string& key, const std::string& hkey, const std::string& value);
    bool HSet(const char* key, const char* hkey, const char* hvalue, size_t hvaluelen);
    std::string HGet(const std::string& key, const std::string& hkey);
    bool HDel(const std::string& key, const std::string& field);
    bool Del(const std::string& key);
    bool ExistsKey(const std::string& key);

    void Close() {
        if (_con_pool) {
            _con_pool->Close();
            _con_pool->ClearConnections();
        }
    }

private:
    RedisMgr();
    std::unique_ptr<RedisConPool> _con_pool;
};