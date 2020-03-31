# 程序说明
CNotify是这样的一个类：
1. 该类有一个Register接口，可对所有该类及该类的派生类的实例进行注册，注册后的实例均有一个唯一的标识ID (unsigned int)。
2. 该类有一个SendNotify接口，可以对指定标识的实例或所有实例发送通知（通知内容为字符串）。
3. 实例在接收到通知后，都能主动在屏幕上显示自身的标识和该通知。   

请设计并实现CNotify类及以上功能。要求不能改动接口。
接口如下：
```
static void Register(CNotify &rInst);
其中：
rInst为注册的实例。
static void SendNotify(int iType, unsigned int uID, char* pContent);
其中：
iType为指定的发送类型：取值范围为SND_SINGLE和SND_ALL，分别为指定单个发送或全体发送。
uID为SND_SINGLE时指定的单个发送的实例ID，SND_ALL时，该参数无意义。
pContent为发送的通知内容。
```

### 要求：
1. CNotify 作为抽象类，派生出几个类，
2. CNotifyWorker, 他收到消息之后，老实打印信息
3. CNotifyLier    他收到消息之后，把消息逆向输出
4. CNotifyLazyer  他收到消息之后，把消息发送给下一个(CNotify队列中的下一个元素，如果没有下一个，从第一个开始)接受者处理，如果找不到，再自己处理


