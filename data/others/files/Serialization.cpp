﻿#include "Serialization.h"
#include "Basic/Identifier.h"
#include "AST/NestedNameSpecifier.h"
#include "Lex/Preprocessor.h"
#include "Sema/Declarator.h"
#include "AST/ASTContext.h"
#include "Parse/Parser.h"

using namespace NatsuLib;
using namespace NatsuLang;
using namespace Serialization;

namespace
{
	[[noreturn]] void ThrowRejectAst()
	{
		nat_Throw(SerializationException, u8"This ast cannot be serialized."_nv);
	}

	[[noreturn]] void ThrowInvalidData()
	{
		nat_Throw(SerializationException, u8"Invalid data."_nv);
	}

	nString GetQualifiedName(natRefPointer<Declaration::NamedDecl> const& decl)
	{
		nString name = decl->GetName();
		auto parent = decl->GetContext();
		while (parent)
		{
			const auto parentDecl = Declaration::Decl::CastFromDeclContext(parent);
			if (const auto namedDecl = dynamic_cast<Declaration::NamedDecl*>(parentDecl))
			{
				name = namedDecl->GetName() + (u8"."_nv + name);
				parent = namedDecl->GetContext();
			}
			else if (dynamic_cast<Declaration::TranslationUnitDecl*>(parentDecl) || dynamic_cast<Declaration::FunctionDecl*>(parent))
			{
				// 翻译单元即顶层声明上下文，函数内部的定义不具有链接性，到此结束即可
				break;
			}
			else
			{
				nat_Throw(SerializationException, u8"Parent is not a namedDecl."_nv);
			}
		}

		return name;
	}

	class UnresolvedId
		: public natRefObjImpl<UnresolvedId, ASTNode>
	{
	public:
		explicit UnresolvedId(nString name)
			: m_Name{ std::move(name) }
		{
		}

		nStrView GetName() const noexcept
		{
			return m_Name;
		}

	private:
		nString m_Name;
	};
}

BinarySerializationArchiveReader::BinarySerializationArchiveReader(natRefPointer<natBinaryReader> reader)
	: m_Reader{ std::move(reader) }
{
}

BinarySerializationArchiveReader::~BinarySerializationArchiveReader()
{
}

nBool BinarySerializationArchiveReader::ReadSourceLocation(nStrView key, SourceLocation& out)
{
	nat_Throw(NotImplementedException);
}

nBool BinarySerializationArchiveReader::ReadString(nStrView key, nString& out)
{
	const auto size = m_Reader->ReadPod<nuInt>();
	out.Resize(size);
	return m_Reader->GetUnderlyingStream()->ReadBytes(reinterpret_cast<nData>(out.data()), size) == size;
}

nBool BinarySerializationArchiveReader::ReadInteger(nStrView key, nuLong& out, std::size_t widthHint)
{
	// TODO: 注意端序
	out = 0;
	return m_Reader->GetUnderlyingStream()->ReadBytes(reinterpret_cast<nData>(&out), widthHint);
}

nBool BinarySerializationArchiveReader::ReadFloat(nStrView key, nDouble& out, std::size_t widthHint)
{
	out = 0;
	if (widthHint == sizeof(nFloat))
	{
		nFloat value;
		const auto ret = m_Reader->GetUnderlyingStream()->ReadBytes(reinterpret_cast<nData>(&value), sizeof(nFloat));
		out = static_cast<nDouble>(value);
		return ret;
	}

	assert(widthHint == sizeof(nDouble));
	return m_Reader->GetUnderlyingStream()->ReadBytes(reinterpret_cast<nData>(&out), sizeof(nDouble));
}

nBool BinarySerializationArchiveReader::StartReadingEntry(nStrView key, nBool isArray)
{
	if (isArray)
	{
		const auto count = m_Reader->ReadPod<nuInt>();
		m_EntryElementCount.emplace_back(true, static_cast<std::size_t>(count));
	}
	else
	{
		m_EntryElementCount.emplace_back(false, 1);
	}

	return true;
}

nBool BinarySerializationArchiveReader::NextReadingElement()
{
	return true;
}

std::size_t BinarySerializationArchiveReader::GetEntryElementCount()
{
	const auto& back = m_EntryElementCount.back();
	return back.second;
}

void BinarySerializationArchiveReader::EndReadingEntry()
{
	m_EntryElementCount.pop_back();
}

BinarySerializationArchiveWriter::BinarySerializationArchiveWriter(natRefPointer<natBinaryWriter> writer)
	: m_Writer{ std::move(writer) }
{
	if (!m_Writer->GetUnderlyingStream()->CanSeek())
	{
		nat_Throw(SerializationException, u8"Stream should be seekable."_nv);
	}
}

BinarySerializationArchiveWriter::~BinarySerializationArchiveWriter()
{
}

void BinarySerializationArchiveWriter::WriteSourceLocation(nStrView key, SourceLocation const& value)
{
	nat_Throw(NotImplementedException);
}

void BinarySerializationArchiveWriter::WriteString(nStrView key, nStrView value)
{
	const auto size = value.size();
	m_Writer->WritePod(static_cast<nuInt>(size));
	m_Writer->GetUnderlyingStream()->WriteBytes(reinterpret_cast<ncData>(value.data()), size);
}

void BinarySerializationArchiveWriter::WriteInteger(nStrView key, nuLong value, std::size_t widthHint)
{
	// TODO: 注意端序
	m_Writer->GetUnderlyingStream()->WriteBytes(reinterpret_cast<ncData>(&value), widthHint);
}

void BinarySerializationArchiveWriter::WriteFloat(nStrView key, nDouble value, std::size_t widthHint)
{
	// TODO: 注意端序
	if (widthHint == sizeof(nFloat))
	{
		const auto realValue = static_cast<nFloat>(value);
		m_Writer->GetUnderlyingStream()->WriteBytes(reinterpret_cast<ncData>(&realValue), sizeof(nFloat));
	}
	else
	{
		assert(widthHint == sizeof(nDouble));
		m_Writer->GetUnderlyingStream()->WriteBytes(reinterpret_cast<ncData>(&value), sizeof(nDouble));
	}
}

void BinarySerializationArchiveWriter::StartWritingEntry(nStrView key, nBool isArray)
{
	m_EntryElementCount.emplace_back(isArray, m_Writer->GetUnderlyingStream()->GetPosition(), std::size_t{});
	if (isArray)
	{
		m_Writer->WritePod(nuInt{});
	}
}

void BinarySerializationArchiveWriter::NextWritingElement()
{
	++std::get<2>(m_EntryElementCount.back());
}

void BinarySerializationArchiveWriter::EndWritingEntry()
{
	const auto& back = m_EntryElementCount.back();
	if (std::get<0>(back))
	{
		const auto pos = m_Writer->GetUnderlyingStream()->GetPosition();
		m_Writer->GetUnderlyingStream()->SetPositionFromBegin(std::get<1>(back));
		m_Writer->WritePod(static_cast<nuInt>(std::get<2>(back)));
		m_Writer->GetUnderlyingStream()->SetPositionFromBegin(pos);
	}

	m_EntryElementCount.pop_back();
}

Deserializer::Deserializer(Syntax::Parser& parser,
                           natRefPointer<Misc::TextProvider<Statement::Stmt::StmtType>> const& stmtTypeMap,
                           natRefPointer<Misc::TextProvider<Declaration::Decl::DeclType>> const& declTypeMap,
                           natRefPointer<Misc::TextProvider<Type::Type::TypeClass>> const& typeClassMap)
	: m_Parser{ parser }, m_Sema{ parser.GetSema() }, m_IsImporting{ false }
{
	if (stmtTypeMap)
	{
		using UnderlyingType = std::underlying_type_t<Statement::Stmt::StmtType>;
		for (auto i = UnderlyingType{}; i < static_cast<UnderlyingType>(Statement::Stmt::StmtType::LastStmt); ++i)
		{
			const auto stmtType = static_cast<Statement::Stmt::StmtType>(i);
			m_StmtTypeMap.emplace(stmtTypeMap->GetText(stmtType), stmtType);
		}
	}

	if (declTypeMap)
	{
		using UnderlyingType = std::underlying_type_t<Declaration::Decl::DeclType>;
		for (auto i = UnderlyingType{}; i < static_cast<UnderlyingType>(Declaration::Decl::DeclType::LastDecl); ++i)
		{
			const auto declType = static_cast<Declaration::Decl::DeclType>(i);
			m_DeclTypeMap.emplace(declTypeMap->GetText(declType), declType);
		}
	}

	if (typeClassMap)
	{
		using UnderlyingType = std::underlying_type_t<Type::Type::TypeClass>;
		for (auto i = UnderlyingType{}; i < static_cast<UnderlyingType>(Type::Type::TypeClass::TypeLast); ++i)
		{
			const auto typeClass = static_cast<Type::Type::TypeClass>(i);
			m_TypeClassMap.emplace(typeClassMap->GetText(typeClass), typeClass);
		}
	}
}

Deserializer::~Deserializer()
{
	if (!m_UnresolvedDeclFixers.empty())
	{
		ThrowInvalidData();
	}
}

std::size_t Deserializer::StartDeserialize(natRefPointer<ISerializationArchiveReader> archive, nBool isImporting)
{
	m_Archive = std::move(archive);
	m_IsImporting = isImporting;
	m_Sema.PushScope(Semantic::ScopeFlags::DeclarableScope);
	m_PseudoTranslationUnit = make_ref<Declaration::TranslationUnitDecl>(m_Sema.GetASTContext());
	// 假装属于真正的翻译单元，不会真的加进去
	m_PseudoTranslationUnit->SetContext(m_Sema.GetASTContext().GetTranslationUnit().Get());
	m_Sema.PushDeclContext(m_Sema.GetCurrentScope(), m_PseudoTranslationUnit.Get());
	m_Archive->StartReadingEntry(u8"Content"_nv, true);
	return m_Archive->GetEntryElementCount();
}

void Deserializer::EndDeserialize()
{
	m_Archive->EndReadingEntry();

	m_Sema.PopDeclContext();
	m_Sema.PopScope();
	m_PseudoTranslationUnit->RemoveAllDecl();
	m_PseudoTranslationUnit.Reset();
}

ASTNodePtr Deserializer::Deserialize()
{
	ASTNodeType type;
	if (!m_Archive->ReadNumType(u8"AstNodeType"_nv, type))
	{
		ThrowInvalidData();
	}

	switch (type)
	{
	case ASTNodeType::Declaration:
		return DeserializeDecl();
	case ASTNodeType::Statement:
		return DeserializeStmt();
	case ASTNodeType::Type:
		return DeserializeType();
	case ASTNodeType::CompilerAction:
		return DeserializeCompilerAction();
	default:
		assert(!"Invalid type.");
		ThrowInvalidData();
	}
}

ASTNodePtr Deserializer::DeserializeDecl()
{
	Declaration::Decl::DeclType type;
	if (!m_Archive->ReadNumType(u8"Type"_nv, type))
	{
		ThrowInvalidData();
	}

	std::vector<Declaration::AttrPtr> attributes;
	if (!m_Archive->StartReadingEntry(u8"Attributes"_nv, true))
	{
		ThrowInvalidData();
	}

	{
		nString attrName;
		const auto count = m_Archive->GetEntryElementCount();
		attributes.reserve(count);
		for (std::size_t i = 0; i < count; ++i)
		{
			if (!m_Archive->ReadString(u8"Name"_nv, attrName))
			{
				ThrowInvalidData();
			}

			attributes.emplace_back(m_Sema.DeserializeAttribute(attrName, m_Archive));
			m_Archive->NextReadingElement();
		}
	}

	m_Archive->EndReadingEntry();

	switch (type)
	{
	case Declaration::Decl::TranslationUnit:
	case Declaration::Decl::Empty:
	case Declaration::Decl::Import:
		break;
	default:
		if (type >= Declaration::Decl::FirstNamed && type <= Declaration::Decl::LastNamed)
		{
			nString name;
			if (!m_Archive->ReadString(u8"Name"_nv, name))
			{
				ThrowInvalidData();
			}

			auto id = getId(name);

			switch (type)
			{
			case Declaration::Decl::Alias:
			{
				if (!m_Archive->StartReadingEntry(u8"AliasAs"_nv))
				{
					ThrowInvalidData();
				}

				auto aliasAs = Deserialize();

				m_Archive->EndReadingEntry();

				auto ret = m_Sema.ActOnAliasDeclaration(m_Sema.GetCurrentScope(), {}, std::move(id), {}, aliasAs);

				if (const auto& unresolved = aliasAs.Cast<UnresolvedId>())
				{
					m_UnresolvedDeclFixers[unresolved->GetName()].emplace(ret, [ret](natRefPointer<Declaration::NamedDecl> decl)
					{
						if (const auto tag = decl.Cast<Declaration::TypeDecl>())
						{
							ret->SetAliasAsAst(tag->GetTypeForDecl());
						}
						else
						{
							ret->SetAliasAsAst(decl);
						}
					});
				}

				tryResolve(ret);
				for (auto attr : attributes)
				{
					ret->AttachAttribute(std::move(attr));
				}
				return ret;
			}
			case Declaration::Decl::Label:
				break;
			case Declaration::Decl::Module:
			{
				auto module = m_Sema.ActOnModuleDecl(m_Sema.GetCurrentScope(), {}, std::move(id));
				{
					Syntax::Parser::ParseScope moduleScope{
						&m_Parser, Semantic::ScopeFlags::DeclarableScope | Semantic::ScopeFlags::ModuleScope
					};
					m_Sema.ActOnStartModule(m_Sema.GetCurrentScope(), module);
					if (!m_Archive->StartReadingEntry(u8"Members"_nv, true))
					{
						ThrowInvalidData();
					}

					const auto count = m_Archive->GetEntryElementCount();
					for (std::size_t i = 0; i < count; ++i)
					{
						auto ast = Deserialize();
						m_Archive->NextReadingElement();
					}

					m_Archive->EndReadingEntry();
					m_Sema.ActOnFinishModule();
				}

				tryResolve(module);
				for (auto attr : attributes)
				{
					module->AttachAttribute(std::move(attr));
				}
				return module;
			}
			case Declaration::Decl::Enum:
			case Declaration::Decl::Class:
			{
				Type::TagType::TagTypeClass tagType;

				if (!m_Archive->ReadNumType(u8"TagType"_nv, tagType))
				{
					ThrowInvalidData();
				}

				auto tagDecl = m_Sema.ActOnTag(m_Sema.GetCurrentScope(), tagType, {}, Specifier::Access::None, std::move(id), {});
				if (type == Declaration::Decl::Class)
				{
					tagDecl->SetTypeForDecl(make_ref<Type::ClassType>(tagDecl));
				}
				else
				{
					tagDecl->SetTypeForDecl(make_ref<Type::EnumType>(tagDecl));
				}

				{
					auto flag = Semantic::ScopeFlags::DeclarableScope;
					if (type == Declaration::Decl::Class)
					{
						flag |= Semantic::ScopeFlags::ClassScope;
					}
					else
					{
						assert(type == Declaration::Decl::Enum);
						flag |= Semantic::ScopeFlags::EnumScope;
					}

					Syntax::Parser::ParseScope tagScope{ &m_Parser, flag };
					m_Sema.ActOnTagStartDefinition(m_Sema.GetCurrentScope(), tagDecl);

					if (!m_Archive->StartReadingEntry(u8"Members"_nv, true))
					{
						ThrowInvalidData();
					}

					const auto count = m_Archive->GetEntryElementCount();
					for (std::size_t i = 0; i < count; ++i)
					{
						auto ast = Deserialize();
						m_Archive->NextReadingElement();
					}

					m_Archive->EndReadingEntry();
					m_Sema.ActOnTagFinishDefinition();
				}

				if (tagType == Type::TagType::TagTypeClass::Enum)
				{
					if (!m_Archive->StartReadingEntry(u8"UnderlyingType"_nv))
					{
						ThrowInvalidData();
					}
					auto underlyingType = Deserialize();
					if (!underlyingType)
					{
						ThrowInvalidData();
					}

					assert(type == Declaration::Decl::Enum);
					auto enumDecl = tagDecl.UnsafeCast<Declaration::EnumDecl>();
					if (const auto& unresolved = underlyingType.Cast<UnresolvedId>())
					{
						m_UnresolvedDeclFixers[unresolved->GetName()].emplace(
							enumDecl, [enumDecl](natRefPointer<Declaration::NamedDecl> const& decl)
							{
								enumDecl->SetUnderlyingType(decl.Cast<Declaration::TypeDecl>()->GetTypeForDecl());
							});
					}
					else
					{
						enumDecl->SetUnderlyingType(std::move(underlyingType));
					}
				}

				tryResolve(tagDecl);
				for (auto attr : attributes)
				{
					tagDecl->AttachAttribute(std::move(attr));
				}
				return tagDecl;
			}
			case Declaration::Decl::Unresolved:
				break;
			default:
				if (type >= Declaration::Decl::FirstValue && type <= Declaration::Decl::LastValue)
				{
					if (!m_Archive->StartReadingEntry(u8"DeclType"_nv))
					{
						ThrowInvalidData();
					}
					Type::TypePtr valueType = Deserialize();
					if (!valueType)
					{
						ThrowInvalidData();
					}
					m_Archive->EndReadingEntry();

					auto dc = Declaration::Decl::CastToDeclContext(m_Sema.GetDeclContext().Get());

					switch (type)
					{
					case Declaration::Decl::Field:
					{
						auto fieldDecl = make_ref<Declaration::FieldDecl>(Declaration::Decl::Field, dc, SourceLocation{},
						                                                  SourceLocation{}, std::move(id), std::move(valueType));
						m_Sema.PushOnScopeChains(fieldDecl, m_Sema.GetCurrentScope());
						tryResolve(fieldDecl);
						for (auto attr : attributes)
						{
							fieldDecl->AttachAttribute(std::move(attr));
						}
						return fieldDecl;
					}
					case Declaration::Decl::EnumConstant:
					{
						nuLong value;
						if (!m_Archive->ReadNumType(u8"Value"_nv, value))
						{
							ThrowInvalidData();
						}
						const auto enumeratorDecl = make_ref<Declaration::EnumConstantDecl>(dc, SourceLocation{}, std::move(id),
						                                                                    std::move(valueType), nullptr, value);
						m_Sema.PushOnScopeChains(enumeratorDecl, m_Sema.GetCurrentScope());
						tryResolve(enumeratorDecl);
						for (auto attr : attributes)
						{
							enumeratorDecl->AttachAttribute(std::move(attr));
						}
						return enumeratorDecl;
					}
					default:
						if (type >= Declaration::Decl::FirstVar && type <= Declaration::Decl::LastVar)
						{
							natRefPointer<Declaration::VarDecl> decl;

							Specifier::StorageClass storageClass;
							if (!m_Archive->ReadNumType(u8"StorageClass"_nv, storageClass))
							{
								ThrowInvalidData();
							}

							Expression::ExprPtr initializer;

							nBool hasInitializer;
							if (m_Archive->ReadBool(u8"HasInitializer"_nv, hasInitializer))
							{
								if (hasInitializer)
								{
									if (m_Archive->StartReadingEntry(u8"Initializer"_nv))
									{
										initializer = Deserialize();
										if (!initializer)
										{
											ThrowInvalidData();
										}
										m_Archive->EndReadingEntry();
									}
									else
									{
										ThrowInvalidData();
									}
								}
							}

							if (m_IsImporting)
							{
								if (storageClass != Specifier::StorageClass::Static)
								{
									// 正在导入外部翻译单元的变量/函数，强制使用 extern 并清除初始化器
									storageClass = Specifier::StorageClass::Extern;
									initializer = nullptr;
								}
							}

							auto addToContext = true;

							switch (type)
							{
							case Declaration::Decl::Var:
								decl = make_ref<Declaration::VarDecl>(Declaration::Decl::Var, dc, SourceLocation{},
								                                      SourceLocation{}, std::move(id), std::move(valueType),
								                                      storageClass);
								if (initializer)
								{
									decl->SetInitializer(std::move(initializer));
								}
								break;
							case Declaration::Decl::ImplicitParam:
								nat_Throw(NotImplementedException);
							case Declaration::Decl::ParmVar:
								decl = make_ref<Declaration::ParmVarDecl>(Declaration::Decl::ParmVar,
								                                          m_Sema.GetASTContext().GetTranslationUnit().Get(), SourceLocation{},
								                                          SourceLocation{},
								                                          std::move(id), std::move(valueType), Specifier::StorageClass::None,
								                                          initializer);
								addToContext = false;
								break;
							default:
								if (type >= Declaration::Decl::FirstFunction && type <= Declaration::Decl::LastFunction)
								{
									natRefPointer<Declaration::FunctionDecl> funcDecl;

									switch (type)
									{
									case Declaration::Decl::Function:
										funcDecl = make_ref<Declaration::FunctionDecl>(Declaration::Decl::Function, dc,
										                                               SourceLocation{}, SourceLocation{}, std::move(id),
										                                               std::move(valueType),
										                                               storageClass);
										break;
									case Declaration::Decl::Method:
										funcDecl = make_ref<Declaration::MethodDecl>(Declaration::Decl::Method, dc,
										                                             SourceLocation{}, SourceLocation{}, std::move(id),
										                                             std::move(valueType),
										                                             storageClass);
										break;
									case Declaration::Decl::Constructor:
										funcDecl = make_ref<Declaration::ConstructorDecl>(dc, SourceLocation{},
										                                                  std::move(id), std::move(valueType), storageClass);
										break;
									case Declaration::Decl::Destructor:
										funcDecl = make_ref<Declaration::DestructorDecl>(dc, SourceLocation{},
										                                                 std::move(id), std::move(valueType), storageClass);
										break;
									default:
										ThrowInvalidData();
									}

									if (!m_Archive->StartReadingEntry(u8"Params"_nv, true))
									{
										ThrowInvalidData();
									}

									std::vector<natRefPointer<Declaration::ParmVarDecl>> params;

									const auto paramCount = m_Archive->GetEntryElementCount();

									params.reserve(paramCount);

									{
										Syntax::Parser::ParseScope prototypeScope{
											&m_Parser,
											Semantic::ScopeFlags::FunctionDeclarationScope | Semantic::ScopeFlags::DeclarableScope | Semantic::ScopeFlags
											::FunctionPrototypeScope
										};

										for (std::size_t i = 0; i < paramCount; ++i)
										{
											if (auto param = Deserialize().Cast<Declaration::ParmVarDecl>())
											{
												params.emplace_back(std::move(param));
											}
											else
											{
												ThrowInvalidData();
											}
											m_Archive->NextReadingElement();
										}
									}

									funcDecl->SetParams(from(params));

									m_Archive->EndReadingEntry();

									Statement::StmtPtr body;
									nBool hasBody;
									if (m_Archive->ReadBool(u8"HasBody"_nv, hasBody))
									{
										if (hasBody)
										{
											if (m_Archive->StartReadingEntry(u8"Body"_nv))
											{
												{
													Syntax::Parser::ParseScope bodyScope{
														&m_Parser,
														Semantic::ScopeFlags::FunctionScope | Semantic::ScopeFlags::DeclarableScope | Semantic::ScopeFlags::
														CompoundStmtScope
													};
													m_Sema.PushDeclContext(m_Sema.GetCurrentScope(), funcDecl.Get());

													for (auto param : params)
													{
														m_Sema.PushOnScopeChains(std::move(param), m_Sema.GetCurrentScope());
													}

													body = Deserialize().Cast<Statement::Stmt>();
													m_Sema.PopDeclContext();
												}

												if (!body)
												{
													ThrowInvalidData();
												}
												m_Archive->EndReadingEntry();
											}
											else
											{
												ThrowInvalidData();
											}
										}
									}

									funcDecl->SetBody(std::move(body));
									decl = std::move(funcDecl);
									break;
								}

								ThrowInvalidData();
							}

							// 不导入内部链接性的声明
							if (m_IsImporting && storageClass == Specifier::StorageClass::Static)
							{
								return nullptr;
							}

							m_Sema.PushOnScopeChains(decl, m_Sema.GetCurrentScope(), addToContext);
							tryResolve(decl);
							for (auto attr : attributes)
							{
								decl->AttachAttribute(std::move(attr));
							}
							return decl;
						}
						break;
					}
				}
				break;
			}
		}
		break;
	}

	nat_Throw(NotImplementedException);
}

ASTNodePtr Deserializer::DeserializeStmt()
{
	Statement::Stmt::StmtType type;
	if (!m_Archive->ReadNumType(u8"Type"_nv, type))
	{
		ThrowInvalidData();
	}

	switch (type)
	{
	case Statement::Stmt::BreakStmtClass:
		break;
	case Statement::Stmt::CatchStmtClass:
		break;
	case Statement::Stmt::TryStmtClass:
		break;
	case Statement::Stmt::CompoundStmtClass:
		break;
	case Statement::Stmt::ContinueStmtClass:
		break;
	case Statement::Stmt::DeclStmtClass:
		break;
	case Statement::Stmt::DoStmtClass:
		break;
	case Statement::Stmt::ConditionalOperatorClass:
		break;
	case Statement::Stmt::ArraySubscriptExprClass:
		break;
	case Statement::Stmt::BinaryOperatorClass:
		break;
	case Statement::Stmt::CompoundAssignOperatorClass:
		break;
	case Statement::Stmt::BooleanLiteralClass:
		break;
	case Statement::Stmt::ConstructExprClass:
		break;
	case Statement::Stmt::DeleteExprClass:
		break;
	case Statement::Stmt::NewExprClass:
		break;
	case Statement::Stmt::ThisExprClass:
		break;
	case Statement::Stmt::ThrowExprClass:
		break;
	case Statement::Stmt::CallExprClass:
		break;
	case Statement::Stmt::MemberCallExprClass:
		break;
	case Statement::Stmt::AsTypeExprClass:
		break;
	case Statement::Stmt::ImplicitCastExprClass:
		break;
	case Statement::Stmt::InitListExprClass:
		break;
	case Statement::Stmt::CharacterLiteralClass:
		break;
	case Statement::Stmt::DeclRefExprClass:
		break;
	case Statement::Stmt::FloatingLiteralClass:
		break;
	case Statement::Stmt::IntegerLiteralClass:
		break;
	case Statement::Stmt::MemberExprClass:
		break;
	case Statement::Stmt::ParenExprClass:
		break;
	case Statement::Stmt::StmtExprClass:
		break;
	case Statement::Stmt::StringLiteralClass:
		break;
	case Statement::Stmt::UnaryOperatorClass:
		break;
	case Statement::Stmt::ForStmtClass:
		break;
	case Statement::Stmt::GotoStmtClass:
		break;
	case Statement::Stmt::IfStmtClass:
		break;
	case Statement::Stmt::LabelStmtClass:
		break;
	case Statement::Stmt::NullStmtClass:
		break;
	case Statement::Stmt::ReturnStmtClass:
		break;
	case Statement::Stmt::CaseStmtClass:
		break;
	case Statement::Stmt::DefaultStmtClass:
		break;
	case Statement::Stmt::SwitchStmtClass:
		break;
	case Statement::Stmt::WhileStmtClass:
		break;
	default:
		break;
	}

	nat_Throw(NotImplementedException);
}

ASTNodePtr Deserializer::DeserializeType()
{
	Type::Type::TypeClass type;
	if (!m_Archive->ReadNumType(u8"Type"_nv, type))
	{
		ThrowInvalidData();
	}

	switch (type)
	{
	case Type::Type::Builtin:
	{
		Type::BuiltinType::BuiltinClass builtinType;
		if (!m_Archive->ReadNumType(u8"BuiltinType"_nv, builtinType))
		{
			ThrowInvalidData();
		}

		return m_Sema.GetASTContext().GetBuiltinType(builtinType);
	}
	case Type::Type::Pointer:
	{
		m_Archive->StartReadingEntry(u8"PointeeType"_nv);
		auto pointeeType = Deserialize();
		if (!pointeeType)
		{
			ThrowInvalidData();
		}

		if (const auto unresolved = pointeeType.Cast<UnresolvedId>())
		{
			auto retType = m_Sema.GetASTContext().GetPointerType(getUnresolvedType(unresolved->GetName()));
			m_UnresolvedDeclFixers[unresolved->GetName()].emplace(
				retType, [this, retType](natRefPointer<Declaration::NamedDecl> const& decl)
				{
					m_Sema.GetASTContext().EraseType(retType);
					m_Sema.ClearTypeNameCache(retType);
					retType->SetPointeeType(decl.Cast<Declaration::TypeDecl>()->GetTypeForDecl());
					m_Sema.GetASTContext().CacheType(retType);
				});
			return retType;
		}

		m_Archive->EndReadingEntry();
		return m_Sema.GetASTContext().GetPointerType(std::move(pointeeType));
	}
	case Type::Type::Array:
	{
		m_Archive->StartReadingEntry(u8"ElementType"_nv);
		auto elementType = Deserialize();
		if (!elementType)
		{
			ThrowInvalidData();
		}
		m_Archive->EndReadingEntry();
		nuLong arraySize;
		if (!m_Archive->ReadNumType(u8"ArraySize"_nv, arraySize))
		{
			ThrowInvalidData();
		}

		if (const auto unresolved = elementType.Cast<UnresolvedId>())
		{
			auto retType = m_Sema.GetASTContext().GetArrayType(getUnresolvedType(unresolved->GetName()), arraySize);
			m_UnresolvedDeclFixers[unresolved->GetName()].emplace(
				retType, [this, retType](natRefPointer<Declaration::NamedDecl> const& decl)
				{
					m_Sema.GetASTContext().EraseType(retType);
					m_Sema.ClearTypeNameCache(retType);
					retType->SetElementType(decl.Cast<Declaration::TypeDecl>()->GetTypeForDecl());
					m_Sema.GetASTContext().CacheType(retType);
				});
			return retType;
		}

		return m_Sema.GetASTContext().GetArrayType(std::move(elementType), arraySize);
	}
	case Type::Type::Function:
	{
		if (!m_Archive->StartReadingEntry(u8"ResultType"_nv))
		{
			ThrowInvalidData();
		}
		auto resultType = Deserialize().Cast<Type::Type>();
		if (!resultType)
		{
			ThrowInvalidData();
		}
		m_Archive->EndReadingEntry();

		std::vector<Type::TypePtr> args;
		m_Archive->StartReadingEntry(u8"ArgType"_nv, true);
		if (const auto count = m_Archive->GetEntryElementCount())
		{
			args.reserve(count);
			for (std::size_t i = 0; i < count; ++i)
			{
				if (auto argType = Deserialize().Cast<Type::Type>())
				{
					args.emplace_back(std::move(argType));
				}
				m_Archive->NextReadingElement();
			}
		}
		m_Archive->EndReadingEntry();

		nBool hasVarArg;
		if (!m_Archive->ReadBool(u8"HasVarArg"_nv, hasVarArg))
		{
			ThrowInvalidData();
		}

		return m_Sema.GetASTContext().GetFunctionType(from(args), std::move(resultType), hasVarArg);
	}
	case Type::Type::Paren:
	{
		if (!m_Archive->StartReadingEntry(u8"InnerType"_nv))
		{
			ThrowInvalidData();
		}
		auto innerType = Deserialize();
		if (!innerType)
		{
			ThrowInvalidData();
		}

		if (const auto unresolved = innerType.Cast<UnresolvedId>())
		{
			auto retType = m_Sema.GetASTContext().GetParenType(getUnresolvedType(unresolved->GetName()));
			m_UnresolvedDeclFixers[unresolved->GetName()].emplace(
				retType, [this, retType](natRefPointer<Declaration::NamedDecl> const& decl)
				{
					m_Sema.GetASTContext().EraseType(retType);
					m_Sema.ClearTypeNameCache(retType);
					retType->SetInnerType(decl.Cast<Declaration::TypeDecl>()->GetTypeForDecl());
					m_Sema.GetASTContext().CacheType(retType);
				});
			return retType;
		}

		m_Archive->EndReadingEntry();
		return m_Sema.GetASTContext().GetParenType(std::move(innerType));
	}
	case Type::Type::Class:
	case Type::Type::Enum:
	{
		nString tagDeclName;
		if (!m_Archive->ReadString(u8"TagDecl"_nv, tagDeclName))
		{
			ThrowInvalidData();
		}

		const auto tagDecl = parseQualifiedName(tagDeclName).Cast<Declaration::TagDecl>();
		if (!tagDecl)
		{
			return make_ref<UnresolvedId>(std::move(tagDeclName));
		}

		return tagDecl->GetTypeForDecl();
	}
	case Type::Type::Auto:
	{
		if (!m_Archive->StartReadingEntry(u8"DeducedAs"_nv))
		{
			ThrowInvalidData();
		}
		auto deducedAsType = Deserialize();
		if (!deducedAsType)
		{
			ThrowInvalidData();
		}

		if (const auto unresolved = deducedAsType.Cast<UnresolvedId>())
		{
			auto retType = m_Sema.GetASTContext().GetAutoType(getUnresolvedType(unresolved->GetName()));
			m_UnresolvedDeclFixers[unresolved->GetName()].emplace(
				retType, [this, retType](natRefPointer<Declaration::NamedDecl> const& decl)
				{
					m_Sema.GetASTContext().EraseType(retType);
					m_Sema.ClearTypeNameCache(retType);
					retType->SetDeducedAsType(decl.Cast<Declaration::TypeDecl>()->GetTypeForDecl());
					m_Sema.GetASTContext().CacheType(retType);
				});
			return retType;
		}

		m_Archive->EndReadingEntry();
		return m_Sema.GetASTContext().GetAutoType(std::move(deducedAsType));
	}
	case Type::Type::Unresolved:
		nat_Throw(NotImplementedException);
	default:
		break;
	}

	nat_Throw(NotImplementedException);
}

ASTNodePtr Deserializer::DeserializeCompilerAction()
{
	nString name;
	if (!m_Archive->ReadString(u8"Name"_nv, name))
	{
		ThrowInvalidData();
	}

	const auto scope = make_scope([this, oldLexer = m_Parser.GetPreprocessor().GetLexer()]() mutable
	{
		m_Parser.GetPreprocessor().SetLexer(std::move(oldLexer));
	});

	m_Parser.GetPreprocessor().SetLexer(make_ref<Lex::Lexer>(0, name, m_Parser.GetPreprocessor()));
	m_Parser.ConsumeToken();
	return m_Parser.ParseCompilerActionName();
}

Identifier::IdPtr Deserializer::getId(nStrView name) const
{
	Lex::Token dummy;
	return m_Sema.GetPreprocessor().FindIdentifierInfo(name, dummy);
}

natRefPointer<Declaration::NamedDecl> Deserializer::parseQualifiedName(nStrView name)
{
	const auto scope = make_scope(
		[this, oldLexer = m_Parser.GetPreprocessor().GetLexer(), diagEnabled = m_Sema.GetDiagnosticsEngine().IsDiagEnabled()
		]() mutable
		{
			m_Parser.GetPreprocessor().SetLexer(std::move(oldLexer));
			m_Sema.GetDiagnosticsEngine().EnableDiag(diagEnabled);
		});

	// 阻止在查找中报错，因为将会在之后进行解析
	m_Sema.GetDiagnosticsEngine().EnableDiag(false);

	m_Parser.GetPreprocessor().SetLexer(make_ref<Lex::Lexer>(0, name, m_Parser.GetPreprocessor()));
	m_Parser.ConsumeToken();
	auto qualifiedId = m_Parser.ParseMayBeQualifiedId();

	if (const auto type = m_Sema.LookupTypeName(qualifiedId.second.first, {},
	                                            m_Sema.GetCurrentScope(), qualifiedId.first))
	{
		if (const auto tagType = type.Cast<Type::TagType>())
		{
			return tagType->GetDecl();
		}

		ThrowInvalidData();
	}

	if (auto mayBeIdExpr = m_Sema.ActOnIdExpr(m_Sema.GetCurrentScope(), qualifiedId.first,
	                                          std::move(qualifiedId.second.first), qualifiedId.second.second, false,
	                                          nullptr))
	{
		if (const auto declRefExpr = mayBeIdExpr.Cast<Expression::DeclRefExpr>())
		{
			return declRefExpr->GetDecl();
		}

		if (const auto memberRefExpr = mayBeIdExpr.Cast<Expression::MemberExpr>())
		{
			return memberRefExpr->GetMemberDecl();
		}

		ThrowInvalidData();
	}

	return nullptr;
}

natRefPointer<Type::UnresolvedType> Deserializer::getUnresolvedType(nStrView name)
{
	Lex::Token token;
	static_cast<void>(m_Parser.GetPreprocessor().FindIdentifierInfo(name, token));
	return m_Sema.GetASTContext().GetUnresolvedType({ token });
}

void Deserializer::tryResolve(natRefPointer<Declaration::NamedDecl> const& namedDecl)
{
	if (const auto iter = m_UnresolvedDeclFixers.find(GetQualifiedName(namedDecl)); iter != m_UnresolvedDeclFixers.end())
	{
		for (const auto& fixer : iter->second)
		{
			fixer.second(namedDecl);
		}

		m_UnresolvedDeclFixers.erase(iter);
	}
}

Serializer::Serializer(Semantic::Sema& sema,
                       natRefPointer<Misc::TextProvider<Statement::Stmt::StmtType>> stmtTypeMap,
                       natRefPointer<Misc::TextProvider<Declaration::Decl::DeclType>> declTypeMap,
                       natRefPointer<Misc::TextProvider<Type::Type::TypeClass>> typeClassMap)
	: m_Sema{ sema }, m_StmtTypeMap{ std::move(stmtTypeMap) }, m_DeclTypeMap{ std::move(declTypeMap) },
	  m_TypeClassMap{ std::move(typeClassMap) }, m_IsExporting{ false }
{
}

Serializer::~Serializer()
{
}

void Serializer::StartSerialize(natRefPointer<ISerializationArchiveWriter> archive, nBool isExporting)
{
	m_Archive = std::move(archive);
	m_IsExporting = isExporting;
	m_Archive->StartWritingEntry(u8"Content"_nv, true);
}

void Serializer::EndSerialize()
{
	m_Archive->EndWritingEntry();
}

void Serializer::VisitCatchStmt(natRefPointer<Statement::CatchStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitTryStmt(natRefPointer<Statement::TryStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitCompoundStmt(natRefPointer<Statement::CompoundStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Content"_nv, true);
	for (const auto& s : stmt->GetChildrenStmt())
	{
		StmtVisitor::Visit(s);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitDeclStmt(natRefPointer<Statement::DeclStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Decl"_nv);
	DeclVisitor::Visit(stmt->GetDecl());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitDoStmt(natRefPointer<Statement::DoStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Body"_nv);
	StmtVisitor::Visit(stmt->GetBody());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Cond"_nv);
	StmtVisitor::Visit(stmt->GetCond());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitExpr(natRefPointer<Expression::Expr> const& expr)
{
	VisitStmt(expr);
	m_Archive->StartWritingEntry(u8"ExprType"_nv);
	TypeVisitor::Visit(expr->GetExprType());
	m_Archive->EndWritingEntry();
	m_Archive->WriteNumType(u8"ValueCategory"_nv, expr->GetValueCategory());
}

void Serializer::VisitConditionalOperator(natRefPointer<Expression::ConditionalOperator> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"Cond"_nv);
	StmtVisitor::Visit(expr->GetCondition());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"LeftOperand"_nv);
	StmtVisitor::Visit(expr->GetLeftOperand());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"RightOperand"_nv);
	StmtVisitor::Visit(expr->GetRightOperand());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitArraySubscriptExpr(natRefPointer<Expression::ArraySubscriptExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"LeftOperand"_nv);
	StmtVisitor::Visit(expr->GetLeftOperand());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"RightOperand"_nv);
	StmtVisitor::Visit(expr->GetRightOperand());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitBinaryOperator(natRefPointer<Expression::BinaryOperator> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"OpCode"_nv, expr->GetOpcode());
	m_Archive->StartWritingEntry(u8"LeftOperand"_nv);
	StmtVisitor::Visit(expr->GetLeftOperand());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"RightOperand"_nv);
	StmtVisitor::Visit(expr->GetRightOperand());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitBooleanLiteral(natRefPointer<Expression::BooleanLiteral> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteBool(u8"Value"_nv, expr->GetValue());
}

void Serializer::VisitConstructExpr(natRefPointer<Expression::ConstructExpr> const& expr)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitDeleteExpr(natRefPointer<Expression::DeleteExpr> const& expr)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitNewExpr(natRefPointer<Expression::NewExpr> const& expr)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitThisExpr(natRefPointer<Expression::ThisExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteBool(u8"IsImplicit"_nv, expr->IsImplicit());
}

void Serializer::VisitThrowExpr(natRefPointer<Expression::ThrowExpr> const& expr)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitCallExpr(natRefPointer<Expression::CallExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"Callee"_nv);
	StmtVisitor::Visit(expr->GetCallee());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Args"_nv, true);
	for (const auto& arg : expr->GetArgs())
	{
		StmtVisitor::Visit(arg);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitCastExpr(natRefPointer<Expression::CastExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"CastType"_nv, expr->GetCastType());
	m_Archive->StartWritingEntry(u8"Operand"_nv);
	StmtVisitor::Visit(expr->GetOperand());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitInitListExpr(natRefPointer<Expression::InitListExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"InitExprs"_nv, true);
	for (const auto& e : expr->GetInitExprs())
	{
		StmtVisitor::Visit(e);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitCharacterLiteral(natRefPointer<Expression::CharacterLiteral> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"Value"_nv, expr->GetCodePoint());
}

void Serializer::VisitDeclRefExpr(natRefPointer<Expression::DeclRefExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteString(u8"QualifiedName"_nv, GetQualifiedName(expr->GetDecl()));
}

void Serializer::VisitFloatingLiteral(natRefPointer<Expression::FloatingLiteral> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"Value"_nv, expr->GetValue());
}

void Serializer::VisitIntegerLiteral(natRefPointer<Expression::IntegerLiteral> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"Value"_nv, expr->GetValue());
}

void Serializer::VisitMemberExpr(natRefPointer<Expression::MemberExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"Base"_nv);
	StmtVisitor::Visit(expr->GetBase());
	m_Archive->EndWritingEntry();
	m_Archive->WriteString(u8"Name"_nv, expr->GetName()->GetName());
	m_Archive->WriteString(u8"QualifiedName"_nv, GetQualifiedName(expr->GetMemberDecl()));
}

void Serializer::VisitParenExpr(natRefPointer<Expression::ParenExpr> const& expr)
{
	VisitExpr(expr);
	m_Archive->StartWritingEntry(u8"InnerExpr"_nv);
	StmtVisitor::Visit(expr->GetInnerExpr());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitStmtExpr(natRefPointer<Expression::StmtExpr> const& expr)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitStringLiteral(natRefPointer<Expression::StringLiteral> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteString(u8"Value"_nv, expr->GetValue());
}

void Serializer::VisitUnaryOperator(natRefPointer<Expression::UnaryOperator> const& expr)
{
	VisitExpr(expr);
	m_Archive->WriteNumType(u8"OpCode"_nv, expr->GetOpcode());
	m_Archive->StartWritingEntry(u8"Operand"_nv);
	StmtVisitor::Visit(expr->GetOperand());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitForStmt(natRefPointer<Statement::ForStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Init"_nv);
	StmtVisitor::Visit(stmt->GetInit());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Cond"_nv);
	StmtVisitor::Visit(stmt->GetCond());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Inc"_nv);
	StmtVisitor::Visit(stmt->GetInc());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Body"_nv);
	StmtVisitor::Visit(stmt->GetBody());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitGotoStmt(natRefPointer<Statement::GotoStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitIfStmt(natRefPointer<Statement::IfStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Cond"_nv);
	StmtVisitor::Visit(stmt->GetCond());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Then"_nv);
	StmtVisitor::Visit(stmt->GetThen());
	m_Archive->EndWritingEntry();
	if (const auto elseStmt = stmt->GetElse())
	{
		m_Archive->WriteBool(u8"HasElse"_nv, true);
		m_Archive->StartWritingEntry(u8"Else"_nv);
		StmtVisitor::Visit(elseStmt);
		m_Archive->EndWritingEntry();
	}
	else
	{
		m_Archive->WriteBool(u8"HasElse"_nv, false);
	}
}

void Serializer::VisitLabelStmt(natRefPointer<Statement::LabelStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitReturnStmt(natRefPointer<Statement::ReturnStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"ReturnExpr"_nv);
	StmtVisitor::Visit(stmt->GetReturnExpr());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitSwitchCase(natRefPointer<Statement::SwitchCase> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitCaseStmt(natRefPointer<Statement::CaseStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitDefaultStmt(natRefPointer<Statement::DefaultStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitSwitchStmt(natRefPointer<Statement::SwitchStmt> const& stmt)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitWhileStmt(natRefPointer<Statement::WhileStmt> const& stmt)
{
	VisitStmt(stmt);
	m_Archive->StartWritingEntry(u8"Cond"_nv);
	StmtVisitor::Visit(stmt->GetCond());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"Body"_nv);
	StmtVisitor::Visit(stmt->GetBody());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitStmt(natRefPointer<Statement::Stmt> const& stmt)
{
	m_Archive->WriteNumType(u8"AstNodeType"_nv, ASTNodeType::Statement);
	if (m_StmtTypeMap)
	{
		m_Archive->WriteString(u8"Type"_nv, m_StmtTypeMap->GetText(stmt->GetType()));
	}
	else
	{
		m_Archive->WriteNumType(u8"Type"_nv, stmt->GetType());
	}
}

void Serializer::Visit(Statement::StmtPtr const& stmt)
{
	StmtVisitor::Visit(stmt);
	m_Archive->NextWritingElement();
}

void Serializer::VisitImportDecl(natRefPointer<Declaration::ImportDecl> const& decl)
{
	// 不导出 import
}

void Serializer::VisitNamedDecl(natRefPointer<Declaration::NamedDecl> const& decl)
{
	VisitDecl(decl);
	m_Archive->WriteString(u8"Name"_nv, decl->GetName());
}

void Serializer::VisitAliasDecl(natRefPointer<Declaration::AliasDecl> const& decl)
{
	VisitNamedDecl(decl);
	m_Archive->StartWritingEntry(u8"AliasAs"_nv);
	const auto alias = decl->GetAliasAsAst();
	assert(alias);
	if (const auto d = alias.Cast<Declaration::Decl>())
	{
		DeclVisitor::Visit(d);
	}
	else if (const auto s = alias.Cast<Statement::Stmt>())
	{
		StmtVisitor::Visit(s);
	}
	else if (const auto t = alias.Cast<Type::Type>())
	{
		TypeVisitor::Visit(t);
	}
	else
	{
		const auto action = alias.Cast<ICompilerAction>();
		assert(action);
		SerializeCompilerAction(action);
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitLabelDecl(natRefPointer<Declaration::LabelDecl> const& decl)
{
	nat_Throw(NotImplementedException);
}

void Serializer::VisitModuleDecl(natRefPointer<Declaration::ModuleDecl> const& decl)
{
	VisitNamedDecl(decl);
	m_Archive->StartWritingEntry(u8"Members"_nv, true);
	for (const auto& d : decl->GetDecls())
	{
		DeclVisitor::Visit(d);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitTypeDecl(natRefPointer<Declaration::TypeDecl> const& decl)
{
	VisitNamedDecl(decl);
}

void Serializer::VisitTagDecl(natRefPointer<Declaration::TagDecl> const& decl)
{
	VisitTypeDecl(decl);
	m_Archive->WriteNumType(u8"TagType"_nv, decl->GetTagTypeClass());
	m_Archive->StartWritingEntry(u8"Members"_nv, true);
	for (const auto& d : decl->GetDecls())
	{
		DeclVisitor::Visit(d);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::VisitEnumDecl(natRefPointer<Declaration::EnumDecl> const& decl)
{
	VisitTagDecl(decl);
	m_Archive->StartWritingEntry(u8"UnderlyingType"_nv);
	TypeVisitor::Visit(decl->GetUnderlyingType());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitValueDecl(natRefPointer<Declaration::ValueDecl> const& decl)
{
	VisitNamedDecl(decl);
	m_Archive->StartWritingEntry(u8"DeclType"_nv);
	TypeVisitor::Visit(decl->GetValueType());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitFunctionDecl(natRefPointer<Declaration::FunctionDecl> const& decl)
{
	VisitVarDecl(decl);
	m_Archive->StartWritingEntry(u8"Params"_nv, true);
	for (const auto& param : decl->GetParams())
	{
		DeclVisitor::Visit(param);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
	if (const auto body = decl->GetBody(); !m_IsExporting && body)
	{
		m_Archive->WriteBool(u8"HasBody"_nv, true);
		m_Archive->StartWritingEntry(u8"Body"_nv);
		StmtVisitor::Visit(body);
		m_Archive->EndWritingEntry();
	}
	else
	{
		m_Archive->WriteBool(u8"HasBody"_nv, false);
	}
}

void Serializer::VisitVarDecl(natRefPointer<Declaration::VarDecl> const& decl)
{
	VisitDeclaratorDecl(decl);
	m_Archive->WriteNumType(u8"StorageClass"_nv, decl->GetStorageClass());
	if (const auto initializer = decl->GetInitializer(); !m_IsExporting && initializer)
	{
		m_Archive->WriteBool(u8"HasInitializer"_nv, true);
		m_Archive->StartWritingEntry(u8"Initializer"_nv);
		StmtVisitor::Visit(initializer);
		m_Archive->EndWritingEntry();
	}
	else
	{
		m_Archive->WriteBool(u8"HasInitializer"_nv, false);
	}
}

void Serializer::VisitImplicitParamDecl(natRefPointer<Declaration::ImplicitParamDecl> const& decl)
{
	VisitVarDecl(decl);
	m_Archive->WriteNumType(u8"ParamType"_nv, decl->GetParamType());
}

void Serializer::VisitEnumConstantDecl(natRefPointer<Declaration::EnumConstantDecl> const& decl)
{
	VisitValueDecl(decl);
	m_Archive->WriteNumType(u8"Value"_nv, decl->GetValue());
}

void Serializer::VisitTranslationUnitDecl(natRefPointer<Declaration::TranslationUnitDecl> const& decl)
{
	VisitDecl(decl);
}

void Serializer::VisitDecl(Declaration::DeclPtr const& decl)
{
	m_Archive->WriteNumType(u8"AstNodeType"_nv, ASTNodeType::Declaration);
	if (m_DeclTypeMap)
	{
		m_Archive->WriteString(u8"Type"_nv, m_DeclTypeMap->GetText(decl->GetType()));
	}
	else
	{
		m_Archive->WriteNumType(u8"Type"_nv, decl->GetType());
	}

	m_Archive->StartWritingEntry(u8"Attributes"_nv, true);
	for (const auto& attr : decl->GetAllAttributes())
	{
		m_Archive->WriteString(u8"Name"_nv, attr->GetName());
		m_Sema.SerializeAttribute(attr, m_Archive);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
}

void Serializer::Visit(Declaration::DeclPtr const& decl)
{
	DeclVisitor::Visit(decl);
	m_Archive->NextWritingElement();
}

void Serializer::VisitBuiltinType(natRefPointer<Type::BuiltinType> const& type)
{
	VisitType(type);
	m_Archive->WriteNumType(u8"BuiltinType"_nv, type->GetBuiltinClass());
}

void Serializer::VisitPointerType(natRefPointer<Type::PointerType> const& type)
{
	VisitType(type);
	m_Archive->StartWritingEntry(u8"PointeeType"_nv);
	TypeVisitor::Visit(type->GetPointeeType());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitArrayType(natRefPointer<Type::ArrayType> const& type)
{
	VisitType(type);
	m_Archive->StartWritingEntry(u8"ElementType"_nv);
	TypeVisitor::Visit(type->GetElementType());
	m_Archive->EndWritingEntry();
	m_Archive->WriteNumType(u8"ArraySize"_nv, type->GetSize());
}

void Serializer::VisitFunctionType(natRefPointer<Type::FunctionType> const& type)
{
	VisitType(type);
	m_Archive->StartWritingEntry(u8"ResultType"_nv);
	TypeVisitor::Visit(type->GetResultType());
	m_Archive->EndWritingEntry();
	m_Archive->StartWritingEntry(u8"ArgType"_nv, true);
	for (const auto& arg : type->GetParameterTypes())
	{
		TypeVisitor::Visit(arg);
		m_Archive->NextWritingElement();
	}
	m_Archive->EndWritingEntry();
	m_Archive->WriteBool(u8"HasVarArg"_nv, type->HasVarArg());
}

void Serializer::VisitParenType(natRefPointer<Type::ParenType> const& type)
{
	VisitType(type);
	m_Archive->StartWritingEntry(u8"InnerType"_nv);
	TypeVisitor::Visit(type->GetInnerType());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitTagType(natRefPointer<Type::TagType> const& type)
{
	VisitType(type);
	m_Archive->WriteString(u8"TagDecl"_nv, GetQualifiedName(type->GetDecl()));
}

void Serializer::VisitDeducedType(natRefPointer<Type::DeducedType> const& type)
{
	VisitType(type);
	m_Archive->StartWritingEntry(u8"DeducedAs"_nv);
	TypeVisitor::Visit(type->GetDeducedAsType());
	m_Archive->EndWritingEntry();
}

void Serializer::VisitUnresolvedType(natRefPointer<Type::UnresolvedType> const& /*type*/)
{
	ThrowRejectAst();
}

void Serializer::VisitType(Type::TypePtr const& type)
{
	m_Archive->WriteNumType(u8"AstNodeType"_nv, ASTNodeType::Type);
	if (m_TypeClassMap)
	{
		m_Archive->WriteString(u8"Type"_nv, m_TypeClassMap->GetText(type->GetType()));
	}
	else
	{
		m_Archive->WriteNumType(u8"Type"_nv, type->GetType());
	}
}

void Serializer::Visit(Type::TypePtr const& type)
{
	TypeVisitor::Visit(type);
	m_Archive->NextWritingElement();
}

void Serializer::SerializeCompilerAction(natRefPointer<ICompilerAction> const& action)
{
	m_Archive->WriteNumType(u8"AstNodeType"_nv, ASTNodeType::CompilerAction);

	nString qualifiedName = action->GetName();
	auto parent = action->GetParent();
	assert(parent);
	while (true)
	{
		const auto parentRef = parent.Lock();
		if (!parentRef->GetParent())
		{
			break;
		}
		qualifiedName = parentRef->GetName() + (u8"."_nv + qualifiedName);
		parent = parentRef->GetParent();
	}

	m_Archive->WriteString(u8"Name"_nv, qualifiedName);
}
