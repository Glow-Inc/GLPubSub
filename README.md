# GLPubSub

[README in Chinese (中文说明)](#glpubsub-chinese)

GLPubSub is a wrapper of `NSNotificationCenter` to make pub sub easier. It's a category on `NSObject`, so it can be called on any subclass of `NSObject`.

## Installation

### CocoaPods

With CocoaPods, you can simply:

* Add following dependency in your `Podfile`:

```
pod "GLPubSub", "~> 1.0"
```

* Run `pod install` to install GLPubSub.

### Source File

If you're not using CocoaPods to manage libraries, you can also import source file directly.

* Download [latest code](https://github.com/Glow-Inc/GLPubSub/archive/master.zip) and unzip.
* Import `NSObject+GLPubSub.h` and `NSObject+GLPubSub.m` into your project. Remember to tick "Copy items if needed" when importing.

## Usage

Because GLPubSub is based on `NSNotificationCenter` and registered on `[NSNotificationCenter defaultCenter]`, system notifications are also supported, e.g. `UIApplicationDidEnterBackgroundNotification`, `UIApplicationDidBecomeActiveNotification`, `UITextFieldTextDidChangeNotification` etc. But you will lose `userInfo` of the notification.

### Set PubSub Queue

GLPubSub is built on top of [`-addObserverForName:object:queue:usingBlock:`](https://developer.apple.com/library/ios/documentation/Cocoa/Reference/Foundation/Classes/NSNotificationCenter_Class/#//apple_ref/occ/instm/NSNotificationCenter/addObserverForName:object:queue:usingBlock:) method of `NSNotificationCenter`. You can set the queue passed into this method by calling `+setPubSubQueue:` of `NSObject`.

The PubSub queue is `nil` by default, which means event callbacks will be triggered on the queue where the notification is posted. You can explicitly set the queue to `[NSOperationQueue maniQueue]` to make all callbacks triggered on main thread.

```objective-c
[NSObject setPubSubQueue:[NSOperationQueue mainQueue]];
```

### Subscribe to Events with Selector

Most of the time, we use `self` as subscriber:

```objective-c
[self subscribe:@"YourEventName" selector:@selector(yourEventHandler)];
```

You can subscribe to event published by some specified object:

```objective-c
[self subscribe:@"YourEventName" obj:somePublisher selector:@selector(yourEventHandler)];
```

If you want your handler only be triggered once, you can use:

```objective-c
[self subscribeOnce:@"YourEventName" selector:@selector(yourEventHandler)];
```

Then after triggered, the event will be automatically unsubscribed.

Your selector can defined accepting one parameter, if so, a `GLEvent` object representing the event will be passed into your selector.

```objective-c
@interface GLEvent : NSObject
@property (nonatomic, copy) NSString *name;
@property (nonatomic, retain) id obj;
@property (nonatomic, retain) id data;
```

Where, `name` is the event name, `obj` is the publisher who triggered the event and `data` is additional data of the event.

### Subscribe to Events with Handler Block

All the methods are similar to those above, only with selector replaced by handler block.

`GLEventHandler` is defined as:

```objective-c
typedef void (^GLEventHandler)(GLEvent *event);
```

So you can subscribe to some event using syntax like:

```objective-c
__weak __typeof__(self) weakSelf = self;
[self subscribe:UIApplicationDidEnterBackgroundNotification handler:^(GLEvent *event) {
    __strong __typeof__(weakSelf) strongSelf = weakSelf;
    [strongSelf appDidEnterBackground];
}];
```

Weakifying self here is important to avoid retain cycle. Corresponding to methods with selector, there're 4 methods can be used with handler block:

```objective-c
- (id)subscribe:(NSString *)eventName handler:(GLEventHandler)handler;
- (id)subscribe:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;
- (id)subscribeOnce:(NSString *)eventName handler:(GLEventHandler)handler;
- (id)subscribeOnce:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;
```

### Unsubscribe

Unsubscribe one event:

```objective-c
- (void)unsubscribe:(NSString *)eventName;
```

Unsubscribe all subscribed events:

```objective-c
- (void)unsubscribeAll;
```

Although the observers will be deallocated when the instance deallocate, it's still recommended to unsubscribe manually, for example in `-dealloc` method, or sometimes in `-viewDidDisappear` method, according to your requirement.

```objective-c
- (void)dealloc
{
    [self unsubscribeAll];
}
```

### Publish Event

You can simply publish an event without additional data:

```objective-c
[self publish:@"YourEventName"];
```

or with additional data, most of the time a `NSDictionary` object in practice:

```objective-c
[self publish:@"YourEventName" data:@{@"key": value}]
```

## Retain Cycle

Since all observers are retained by `self`, there will be retain cycle if your block retains `self`. So you have to weakify self as mentioned above. We highly recommend doing weakify/strongify with [EXTScope in libextobjc](https://github.com/jspahrsummers/libextobjc/blob/master/extobjc/EXTScope.h), with the library, your code would look like:

```objective-c
@weakify(self);
[self subscribe:UIApplicationDidEnterBackgroundNotification handler:^(GLEvent *event) {
    @strongify(self);
    [self appDidEnterBackground];
}];
```

## License

GLPubSub is available under the MIT license. See the LICENSE file for more info.

---

# GLPubSub (Chinese)

[README in English](#glpubsub)

GLPubSub 是 `NSNotificationCenter` 的封装，目标是简化 iOS 开发中的发布订阅模式。因为是 `NSObject` 的 Category，所以可以在任意 `NSObject` 的子类上使用。

## 安装

### CocoaPods

如果通过 CocoaPods 管理第三方依赖，你可以：

* 在 `Podfile` 里添加以下依赖：

```
pod "GLPubSub", "~> 1.0"
```

* 运行 `pod install` 来安装 GLPubSub

### 源文件

如果你的项目没有用 CocoaPods 来管理第三方依赖，你也可以直接导入源文件。

* 下载[最新代码](https://github.com/Glow-Inc/GLPubSub/archive/master.zip)并解压
* 导入 `NSObject+GLPubSub.h` 和 `NSObject+GLPubSub.m` 到你的工程，记得在导入时勾选 "Copy items if needed"

## 使用

因为 GLPubSub 是基于 `NSNotificationCenter` 并注册在 `[NSNotificationCenter defaultCenter]` 的，所以 GLPubSub 也支持大部分系统通知，例如 `UIApplicationDidEnterBackgroundNotification`，`UIApplicationDidBecomeActiveNotification`，`UITextFieldTextDidChangeNotification` 等等，但是转发的过程中会丢弃系统通知的 `userInfo` 字段。

### 设置 PubSub 的队列

GLPubSub 主要基于 `NSNotificationCenter` 的 [`-addObserverForName:object:queue:usingBlock:`](https://developer.apple.com/library/ios/documentation/Cocoa/Reference/Foundation/Classes/NSNotificationCenter_Class/#//apple_ref/occ/instm/NSNotificationCenter/addObserverForName:object:queue:usingBlock:) 方法。你可以调用 `NSObject` 的 `+setPubSubQueue:` 方法来设置传入该方法的 `queue`。

默认传入的 `queue` 为 `nil`，这意味着所有事件会在发布通知的线程中被执行。你可以手动设置为 `[NSOperationQueue maniQueue]` 使得所有事件在主线程被触发：

```objective-c
[NSObject setPubSubQueue:[NSOperationQueue mainQueue]];
```

### 通过 Selector 订阅事件

大部分时候，我们用 `self` 作为订阅者：

```objective-c
[self subscribe:@"YourEventName" selector:@selector(yourEventHandler)];
```

你也可以指定事件的发布者：

```objective-c
[self subscribe:@"YourEventName" obj:somePublisher selector:@selector(yourEventHandler)];
```

如果你希望你的方法只触发一次，你可以用：

```objective-c
[self subscribeOnce:@"YourEventName" selector:@selector(yourEventHandler)];
```

这样当该事件被触发后，就会自动取消订阅。

你的方法可以接受一个 `GLEvent` 参数，该参数包含了被触发事件的相关信息。

```objective-c
@interface GLEvent : NSObject
@property (nonatomic, copy) NSString *name;
@property (nonatomic, retain) id obj;
@property (nonatomic, retain) id data;
```

`name` 是事件名，`obj` 是发布者，`data` 是附加信息。

### 通过 Block 订阅事件

方法与上面通过 selector 订阅的方法类似：

`GLEventHandler` 定义如下：

```objective-c
typedef void (^GLEventHandler)(GLEvent *event);
```

所以你可以如下用 block 订阅一个事件：

```objective-c
__weak __typeof__(self) weakSelf = self;
[self subscribe:UIApplicationDidEnterBackgroundNotification handler:^(GLEvent *event) {
    __strong __typeof__(weakSelf) strongSelf = weakSelf;
    [strongSelf appDidEnterBackground];
}];
```

这里的 weak 化是为了避免循环引用。对应于前面 selector 的方法，用 block 也有 4 种调用方法：

```objective-c
- (id)subscribe:(NSString *)eventName handler:(GLEventHandler)handler;
- (id)subscribe:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;
- (id)subscribeOnce:(NSString *)eventName handler:(GLEventHandler)handler;
- (id)subscribeOnce:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;
```

### 取消订阅

取消订阅某个事件：

```objective-c
- (void)unsubscribe:(NSString *)eventName;
```

取消订阅所有事件：

```objective-c
- (void)unsubscribeAll;
```

虽然当实例被销毁时，存在 associated object 中的观察者也都会被销毁，但还是建议手动取消订阅，如根据不同需求，在 `-dealloc` 或 `-viewDidDisappear` 方法中取消订阅。

```objective-c
- (void)dealloc
{
    [self unsubscribeAll];
}
```

### 发布事件

你可以简单地发布一个事件：

```objective-c
[self publish:@"YourEventName"];
```

也可以附带一些数据，很多时候我们会传入一个 `NSDictionary` 来附带更多结构化的数据：

```objective-c
[self publish:@"YourEventName" data:@{@"key": value}]
```

## 循环引用

因为所有生成的观察者都会被 `self` 引用，所以当你的 block 引用 `self` 的时候就会形成循环引用导致实例无法被释放，所以你必须 weakify `self`。强烈推荐用 [libextobjc 中的 EXTScope](https://github.com/jspahrsummers/libextobjc/blob/master/extobjc/EXTScope.h) 来做 weakify/strongify：
```objective-c
@weakify(self);
[self subscribe:UIApplicationDidEnterBackgroundNotification handler:^(GLEvent *event) {
    @strongify(self);
    [self appDidEnterBackground];
}];
```

## License

GLPubSub 基于 MIT 协议开源，详见 LICENSE 文件。
