/*  -*- c++ -*-

    c2ffi
    Copyright (C) 2013  Ryan Pavlik

    This file is part of c2ffi.

    c2ffi is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    c2ffi is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with c2ffi.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef C2FFI_DECL_H
#define C2FFI_DECL_H

#include <string>
#include <utility>
#include <vector>
#include <clang/AST/Decl.h>
#include "c2ffi.h"
#include "c2ffi/type.h"

namespace c2ffi {
    class Decl : public Writable {
        std::string _name;
        std::string _loc;
        unsigned int _id;
        unsigned int _nsparent;

    public:
        Decl(std::string name)
            : _name(name), _id(0) { }
        Decl(clang::NamedDecl *d);
        virtual ~Decl() { }

        virtual const std::string& name() const { return _name; }
        virtual const std::string& location() const { return _loc; }

        unsigned int id() const { return _id; }
        void set_id(unsigned int id) { _id = id; }

        unsigned int ns() const { return _nsparent; }
        void set_ns(unsigned int ns) { _nsparent = ns; }

        virtual void set_location(const std::string &loc) { _loc = loc; }
        virtual void set_location(clang::CompilerInstance &ci, const clang::Decl *d);
    };

    class UnhandledDecl : public Decl {
        std::string _kind;
    public:
        UnhandledDecl(std::string name, std::string kind)
            : Decl(name), _kind(kind) { }

        DEFWRITER(UnhandledDecl);
        const std::string& kind() const { return _kind; }
    };

    class TypeDecl : public Decl {
        Type *_type;
    public:
        TypeDecl(std::string name, Type *type)
            : Decl(name), _type(type) { }
        virtual ~TypeDecl() { delete _type; }

        DEFWRITER(TypeDecl);
        virtual const Type& type() const { return *_type; }
    };

    class VarDecl : public TypeDecl {
        std::string _value;
        bool _is_extern;
        bool _is_string;

    public:
        VarDecl(std::string name, Type *type, std::string value = "",
                bool is_extern = false, bool is_string = false)
            : TypeDecl(name, type), _value(value), _is_extern(is_extern),
              _is_string(is_string) { }

        DEFWRITER(VarDecl);

        const std::string& value() const { return _value; }
        bool is_extern() const { return _is_extern; }

        bool is_string() const { return _is_string; }
        bool set_is_string(bool v) { return _is_string = v; }
    };

    class FieldsMixin {
        NameTypeVector _v;
    public:
        virtual ~FieldsMixin();

        void add_field(Name, Type*);
        void add_field(C2FFIASTConsumer *ast, clang::FieldDecl *f);
        void add_field(C2FFIASTConsumer *ast, clang::ParmVarDecl *v);
        const NameTypeVector& fields() const { return _v; }
    };

    class FunctionDecl : public Decl, public FieldsMixin, public TemplateMixin {
        Type *_return;
        bool _is_variadic;
        bool _is_inline;
        bool _is_objc_method;
        bool _is_class_method;

        std::string _storage_class;
    public:
        FunctionDecl(C2FFIASTConsumer *ast,
                     std::string name, Type *type, bool is_variadic,
                     bool is_inline, clang::StorageClass storage_class,
                     const clang::TemplateArgumentList *arglist = NULL);

        DEFWRITER(FunctionDecl);

        virtual const Type& return_type() const { return *_return; }

        bool is_variadic() const { return _is_variadic; }
        bool is_inline() const { return _is_inline; }

        const std::string& storage_class() const { return _storage_class; }

        bool is_objc_method() const { return _is_objc_method; }
        void set_is_objc_method(bool val) {
            _is_objc_method = val;
        }

        bool is_class_method() const { return _is_class_method; }
        void set_is_class_method(bool val) {
            _is_class_method = val;
        }
    };

    typedef std::vector<FunctionDecl*> FunctionVector;

    class FunctionsMixin {
        FunctionVector _v;
    public:
        virtual ~FunctionsMixin();

        void add_function(FunctionDecl *f);
        const FunctionVector& functions() const { return _v; }

        void add_functions(C2FFIASTConsumer *ast, const clang::ObjCContainerDecl *d);
        void add_functions(C2FFIASTConsumer *ast, const clang::CXXRecordDecl *d);
    };

    class TypedefDecl : public TypeDecl {
    public:
        TypedefDecl(std::string name, Type *type)
            : TypeDecl(name, type) { }

        DEFWRITER(TypedefDecl);
    };

    class RecordDecl : public Decl, public FieldsMixin {
        bool _is_union;
        NameTypeVector _v;

        uint64_t _bit_size;
        unsigned _bit_alignment;

    public:
        RecordDecl(std::string name, bool is_union = false)
            : Decl(name), _is_union(is_union), _bit_size(0),
              _bit_alignment(0)
        { }

        DEFWRITER(RecordDecl);
        bool is_union() const { return _is_union; }

        uint64_t bit_size() const { return _bit_size; }
        void set_bit_size(uint64_t size) { _bit_size = size; }

        uint64_t bit_alignment() const { return _bit_alignment; }
        void set_bit_alignment(uint64_t alignment) { _bit_alignment = alignment; }

        void fill_record_decl(C2FFIASTConsumer *ast, const clang::RecordDecl *d);
    };

    class EnumDecl : public Decl {
        NameNumVector _v;
    public:
        EnumDecl(std::string name) : Decl(name) { }

        DEFWRITER(EnumDecl);

        void add_field(std::string, uint64_t);
        const NameNumVector& fields() const { return _v; }
    };

    /** C++ **/
    class CXXRecordDecl : public RecordDecl, public FunctionsMixin,
                          public TemplateMixin {
    public:
        enum Access { access_public = clang::AS_public,
                      access_protected = clang::AS_protected,
                      access_private = clang::AS_private };

        struct ParentRecord {
            std::string name;
            Access access;
            int64_t parent_offset;
            bool is_virtual;

            ParentRecord(const std::string &n, Access ac, int64_t off,
                         bool virt)
                : name(n), access(ac), parent_offset(off),
                  is_virtual(virt) { }
        };

        typedef std::vector<ParentRecord> ParentRecordVector;

    private:
        ParentRecordVector _parents;
        bool _is_abstract;
        bool _is_class;

    public:

        CXXRecordDecl(C2FFIASTConsumer *ast,
                      std::string name, bool is_union = false,
                      bool is_class = false,
                      const clang::TemplateArgumentList *arglist = NULL)
            : RecordDecl(name, is_union), _is_class(is_class),
              TemplateMixin(ast, arglist)
        { }

        DEFWRITER(CXXRecordDecl);

        const ParentRecordVector& parents() const { return _parents; }
        void add_parent(const std::string& name, Access ac,
                        int64_t offset = 0, bool is_virtual = false) {
            _parents.push_back(ParentRecord(name, ac, offset, is_virtual));
        }

        bool is_class() const { return _is_class; }

        bool is_abstract() const { return _is_abstract; }
        void set_is_abstract(bool b) { _is_abstract = b; }
    };

    class CXXFunctionDecl : public FunctionDecl {
        bool _is_static;
        bool _is_virtual;
        bool _is_const;
        bool _is_pure;

    public:
        CXXFunctionDecl(C2FFIASTConsumer *ast,
                        std::string name, Type *type, bool is_variadic,
                        bool is_inline, clang::StorageClass storage_class,
                        const clang::TemplateArgumentList *arglist = NULL)
            : FunctionDecl(ast, name, type, is_variadic, is_inline,
                           storage_class, arglist)
        { }

        DEFWRITER(CXXFunctionDecl);

        bool is_static() const { return _is_static; }
        bool is_virtual() const { return _is_virtual; }
        bool is_const() const { return _is_const; }
        bool is_pure() const { return _is_pure; }

        void set_is_static(bool b) { _is_static = b; }
        void set_is_virtual(bool b) { _is_virtual = b; }
        void set_is_const(bool b) { _is_const = b; }
        void set_is_pure(bool b) { _is_pure = b; }
    };

    class CXXNamespaceDecl : public Decl {
    public:
        CXXNamespaceDecl(std::string name)
            : Decl(name) { }

        DEFWRITER(CXXNamespaceDecl);
    };

    /** ObjC **/
    class ObjCInterfaceDecl : public Decl, public FieldsMixin,
                              public FunctionsMixin {
        std::string _super;
        bool _is_forward;
        NameVector _protocols;
    public:
        ObjCInterfaceDecl(std::string name, std::string super,
                          bool is_forward)
            : Decl(name), _super(super), _is_forward(is_forward) { }

        DEFWRITER(ObjCInterfaceDecl);

        const std::string& super() const { return _super; }
        bool is_forward() const { return _is_forward; }

        void add_protocol(Name proto);
        const NameVector& protocols() const { return _protocols; }
    };

    class ObjCCategoryDecl : public Decl, public FunctionsMixin {
        Name _category;

    public:
        ObjCCategoryDecl(Name name, Name category)
            : Decl(name), _category(category) { }

        DEFWRITER(ObjCCategoryDecl);

        const Name& category() const { return _category; }
    };

    class ObjCProtocolDecl : public Decl, public FunctionsMixin {
    public:
        ObjCProtocolDecl(Name name)
            : Decl(name) { }

        DEFWRITER(ObjCProtocolDecl);
    };
}

#endif /* C2FFI_DECL_H */
