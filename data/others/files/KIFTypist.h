//
//  KIFTypist.h
//  KIF
//
//  Created by Pete Hodgson on 8/12/12.
//
//

@interface KIFTypist : NSObject
+ (UIWindow *) keyboardWindow;
+ (BOOL)enterCharacter:(NSString *)characterString;
+ (BOOL)enterText:(NSString *)text;
+ (void) cancelAnyInitialKeyboardShift;
@end
