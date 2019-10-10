// -- (c) Synack Inc 2016
//
//  ui-kit.h
//
//  Created by Mikhail Sosonkin on 6/9/15.
//
//

#ifndef appMonkey_ui_kit_h
#define appMonkey_ui_kit_h

#import <objc/runtime.h>
#import <UIKit/UIApplication.h>

#if __cplusplus
extern "C" {
#endif

    void walkViewTree(UIView * root, BOOL (^callback)(UIView * curView));
    
    void walkViewRoots(NSArray* roots, BOOL (^callback)(UIView * curView));
    
    void walkUpView(UIView* root, BOOL (^callback)(UIView * curView));

    NSArray* findViewType(Class type, UIView * root);

    void dispatch_on_main(void (^cb)());
#if __cplusplus
}   // Extern C
#endif

#endif
