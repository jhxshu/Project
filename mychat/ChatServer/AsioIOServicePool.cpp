#include "AsioIOServicePool.h"
#include <iostream>
using namespace std;
AsioIOServicePool::AsioIOServicePool(std::size_t size) :_ioServices(size),
_works(size), _nextIOService(0) {
	for (std::size_t i = 0; i < size; ++i) {
		// 传入 executor 而不是直接传 io_context
		_works[i] = std::make_unique<Work>(_ioServices[i].get_executor());
	}

	//遍历多个ioservice，创建多个线程，每个线程内部启动ioservice
	for (std::size_t i = 0; i < _ioServices.size(); ++i) {
		_threads.emplace_back([this, i]() {
			_ioServices[i].run();
			});
	}			
}

AsioIOServicePool::~AsioIOServicePool() {
	Stop();
	std::cout << "AsioIOServicePool destruct" << endl;
}

boost::asio::io_context& AsioIOServicePool::GetIOService() {
	auto& service = _ioServices[_nextIOService++];
	if (_nextIOService == _ioServices.size()) {
		_nextIOService = 0;
	}
	return service;
}

void AsioIOServicePool::Stop() {
	//因为仅仅执行work.reset并不能让iocontext从run的状态中退出
	//当iocontext已经绑定了读或写的监听事件后，还需要手动stop该服务。
	for (auto& work : _works) {
		if (!work) continue; // 防止空指针
		//把服务先停止
		// 获取 executor
		auto executor = work->get_executor();
		// 通过 executor 获取 io_context 并停止
		executor.context().stop();
		work.reset();
	}

	for (auto& t : _threads) {
		if (t.joinable()) t.join();
	}
}