//
//  UIViewController+NGTabBarItem.h
//  NGTabBarController
//
//  Created by Tretter Matthias on 24.04.12.
//  Copyright (c) 2012 NOUS Wissensmanagement GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>


@class NGTabBarItem;
@class NGTabBarController;


@interface UIViewController (NGTabBarItem)

@property (nonatomic, strong, setter = ng_setTabBarItem:) NGTabBarItem *ng_tabBarItem;
@property (nonatomic, readonly) NGTabBarController *ng_tabBarController;

@end
