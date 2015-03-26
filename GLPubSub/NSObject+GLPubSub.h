//
//  NSObject+PubSub.h
//
//  Created by Ryan Ye on 3/20/13.
//  Copyright (c) 2013-2015 Glow, Inc. All rights reserved.
//

#import <Foundation/Foundation.h>

@class GLEvent;

/**
 *  `GLEventHandler` is the type of event handler block
 *
 *  @param event Object for the event which triggers current handler block.
 */
typedef void (^GLEventHandler)(GLEvent *event);

/**
 `GLEvent` is the event object passed into handler block.
 */
@interface GLEvent : NSObject

/**
 *  Name of the event
 */
@property (nonatomic, copy) NSString *name;

/**
 *  The object triggering the event
 */
@property (nonatomic, retain) id obj;

/**
 *  Additional data of the event
 */
@property (nonatomic, retain) id data;

/**
 *  Init `GLEvent` with `name`, `obj` and `data`.
 *
 *  @param name Name of the event
 *  @param obj  The object triggering the event
 *  @param data Additional data of the event
 *
 *  @return Initialized `GLEvent`
 */
- (id)initWithName:(NSString *)name obj:(id)obj data:(id)data;

@end

/**
 *  `GLPubSub` category on `NSObject`, so you can call methods in `GLPubSub` on any subclass of `NSObject`
 */
@interface NSObject (GLPubSub)

#pragma mark - Class Methods
/**
 *  Set the queue to handle events. If not set or set to `nil, events will be handled on the queue where the notification is posted. Otherwise, all the events will be handled on the set queue.
 *
 *  @param queue The queue on which to handle events.
 */
+ (void)setPubSubQueue:(NSOperationQueue *)queue;

#pragma mark - Publish Methods
/**
 *  Publish an event without additional data.
 *
 *  @param name Event name to publish.
 *
 *  @see -publish:data:
 */
- (void)publish:(NSString *)name;

/**
 *  Publish an event with additional data.
 *
 *  @param name Event name to publish.
 *  @param data Additional data.
 *
 *  @see -publish:
 */
- (void)publish:(NSString *)name data:(id)data;

#pragma mark - Subscribe Methods with Selector
/**
 *  Subscribe an event with selector.
 *
 *  @param eventName Event name to subscribe.
 *  @param selector  Selector to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:obj:selector:
 *  @see -subscribeOnce:selector:
 *  @see -subscribeOnce:obj:selector:
 */
- (id)subscribe:(NSString *)eventName selector:(SEL)selector;

/**
 *  Subscribe an event from a specified object with selector.
 *
 *  @param eventName Event name to subscribe.
 *  @param object    Publisher to subscribe from.
 *  @param selector  Selector to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:selector:
 *  @see -subscribeOnce:selector:
 *  @see -subscribeOnce:obj:selector:
 */
- (id)subscribe:(NSString *)eventName obj:(id)obj selector:(SEL)selector;

/**
 *  Subscribe an event only once with selector.
 *
 *  @param eventName Event name to subscribe.
 *  @param selector  Selector to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:selector:
 *  @see -subscribe:obj:selector:
 *  @see -subscribeOnce:obj:selector:
 */
- (id)subscribeOnce:(NSString *)eventName selector:(SEL)selector;

/**
 *  Subscribe an event from a specified object only once with selector.
 *
 *  @param eventName Event name to subscribe.
 *  @param object    Publisher to subscribe from.
 *  @param selector  Selector to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:selector:
 *  @see -subscribe:obj:selector:
 *  @see -subscribeOnce:selector:
 */
- (id)subscribeOnce:(NSString *)eventName obj:(id)obj selector:(SEL)selector;

#pragma mark - Subscribe Methods with Handler Block
/**
 *  Subscribe an event with handler block.
 *
 *  @param eventName Event name to subscribe.
 *  @param handler   Handler block to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:obj:handler:
 *  @see -subscribeOnce:handler:
 *  @see -subscribeOnce:obj:handler:
 */
- (id)subscribe:(NSString *)eventName handler:(GLEventHandler)handler;

/**
 *  Subscribe an event from a specified object with handler block.
 *
 *  @param eventName Event name to subscribe.
 *  @param obj       Publisher to subscribe from.
 *  @param handler   Handler block to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:handler:
 *  @see -subscribeOnce:handler:
 *  @see -subscribeOnce:obj:handler:
 */
- (id)subscribe:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;

/**
 *  Subscribe an event only once with handler block.
 *
 *  @param eventName Event name to subscribe.
 *  @param handler   Handler block to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:handler:
 *  @see -subscribe:obj:handler:
 *  @see -subscribeOnce:obj:handler:
 */
- (id)subscribeOnce:(NSString *)eventName handler:(GLEventHandler)handler;

/**
 *  Subscribe an event from a specified object only once with handler block.
 *
 *  @param eventName Event name to subscribe.
 *  @param obj       Publisher to subscribe from.
 *  @param handler   Handler block to handle the event.
 *
 *  @return An opaque object to act as the observer.
 *
 *  @see -subscribe:handler:
 *  @see -subscribe:obj:handler:
 *  @see -subscribeOnce:handler:
 */
- (id)subscribeOnce:(NSString *)eventName obj:(id)obj handler:(GLEventHandler)handler;

#pragma mark - Unsubscribe Methods
/**
 *  Unsubscribe speficied event for current instance.
 *
 *  @param eventName Name of event to unsubscribe.
 *
 *  @see -unsubscribeAll
 */
- (void)unsubscribe:(NSString *)eventName;

/**
 *  Unsubscribe all events for current instance.
 *
 *  @see -unsubscribe:
 */
- (void)unsubscribeAll;

@end
