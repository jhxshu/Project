# 更新日志

## [2026-03-17]

### 新增

- `mychat/` - 即时通讯软件
  - 登录、注册界面 UI 设计（基于QT平台）
  - 编写 HTTP 管理器（CRTP 实现）
  - QSS 样式切换功能

## [2026-03-18]

### 更新

- `mychat/` - 即时通讯软件
  - 通过Beast实现http get请求处理
  - 通过实现简易修复url函数修复带参数的请求无法解析
  - 通过Beast实现http post请求以发送验证码

## [2026-3-20]

### 更新

- `mychat/` - 即时通讯软件
  - 通过QT发送post请求， 配合Json数据序列化完成发送并收到回包
  - 实现Grpc发送请求的逻辑

## [2026-3-24]

### 更新

- `mychat/` - 即时通讯软件
  - 通过nodejs实现grpc server，并应答grpc client的请求，同时调用nodemail发送邮件
  - 构造Asio的io_context连接池提高并发，将grpc连接改为连接池，提高安全和效率
  - 配置Redis服务，封装Redis连接池
  - 通过nodejs实现邮箱验证和发送验证码功能，初步实现用户注册http联调功能
  - 搭建mysql服务，基于mysql_connector封装操作接口、连接池