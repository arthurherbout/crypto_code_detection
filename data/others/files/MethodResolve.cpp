#include "../classfile/AccessFlag.h"
#include "MethodResolve.h"

//--------------------------------------------------------------------------------
// If C contains a declaration for an instance method m that overrides the
// resolved method, then m is the method to be invoked.
//--------------------------------------------------------------------------------
CallSite findInstanceMethod(const JavaClass *jc, const std::string &methodName,
                            const std::string &methodDescriptor) {
    auto *methodInfo = jc->findMethod(methodName, methodDescriptor);
    if (methodInfo && !IS_METHOD_STATIC(methodInfo->accessFlags)) {
        return CallSite::makeCallSite(jc, methodInfo);
    }
    return CallSite{};
}

//--------------------------------------------------------------------------------
// If C has a superclass, a search for a declaration of an instance
// method that overrides the resolved method is performed, starting with the
// direct superclass of C and continuing with the direct superclass of that
// class, and so forth, until an overriding method is found or no further
// superclasses exist.If an overriding method is found, it is the method to be
// invoked.
//--------------------------------------------------------------------------------
CallSite findInstanceMethodOnSupers(const JavaClass *jc,
                                    const std::string &methodName,
                                    const std::string &methodDescriptor) {
    if (!jc->hasSuperClass()) {
        return CallSite{};
    }

    JavaClass *superClass = yrt.ma->loadClassIfAbsent(jc->getSuperClassName());
    auto methodInfo = superClass->findMethod(methodName, methodDescriptor);
    if (methodInfo && !IS_METHOD_STATIC(methodInfo->accessFlags)) {
        return CallSite::makeCallSite(superClass, methodInfo);
    }

    return findInstanceMethodOnSupers(superClass, methodName, methodDescriptor);
}
//--------------------------------------------------------------------------------
// If C is an interface and the class Object contains a declaration of a public
// instance method with the same name and descriptor as the resolved method,
// then it is the method to be invoked.
//--------------------------------------------------------------------------------
CallSite findJavaLangObjectMethod(const JavaClass *jc,
                                  const std::string &methodName,
                                  const std::string &methodDescriptor) {
    if (IS_CLASS_INTERFACE(jc->getAccessFlag())) {
        JavaClass *jloc = yrt.ma->findJavaClass("java/lang/Object");
        auto *jlom = jloc->findMethod(methodName, methodDescriptor);
        if (jlom && IS_METHOD_PUBLIC(jlom->accessFlags) &&
            !IS_METHOD_STATIC(jlom->accessFlags)) {
            return CallSite::makeCallSite(jloc, jlom);
        }
    }
    return CallSite{};
}
//--------------------------------------------------------------------------------
// If there is exactly one maximally - specific method(5.4.3.3) in the
// superinterfaces of C that matches the resolved method's name and descriptor
// and is not abstract, then it is the method to be invoked.
//--------------------------------------------------------------------------------
CallSite findMaximallySpecifiedMethod(const JavaClass *jc,
                                      const std::string &methodName,
                                      const std::string &methodDescriptor) {
    if (!jc->hasSuperClass()) {
        return CallSite{};
    }
    JavaClass *superClass = yrt.ma->loadClassIfAbsent(jc->getSuperClassName());

    if (superClass->getInterfaceCount()) {
        FOR_EACH(eachInterface, jc->getInterfaceCount()) {
            const std::string &interfaceName =
                jc->getInterfaceClassName(eachInterface);

            JavaClass *interfaceClass =
                yrt.ma->loadClassIfAbsent(interfaceName);
            auto *methodInfo =
                interfaceClass->findMethod(methodName, methodDescriptor);
            if (methodInfo && (!IS_METHOD_ABSTRACT(methodInfo->accessFlags) &&
                               !IS_METHOD_STATIC(methodInfo->accessFlags) &&
                               !IS_METHOD_PRIVATE(methodInfo->accessFlags))) {
                return CallSite::makeCallSite(interfaceClass, methodInfo);
            }
            if (methodInfo && (!IS_METHOD_STATIC(methodInfo->accessFlags) &&
                               !IS_METHOD_PRIVATE(methodInfo->accessFlags))) {
                return CallSite::makeCallSite(interfaceClass, methodInfo);
            }
        }
    }

    return findMaximallySpecifiedMethod(superClass, methodName,
                                        methodDescriptor);
}
