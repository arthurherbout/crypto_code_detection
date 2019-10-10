#include "managed_ptr.h"

#include "builtins.h"
#include "gc/block_list.h"
#include "value/basic_object.h"

using namespace vv;
using namespace gc;

managed_ptr managed_ptr::type() const
{
  switch (m_tag) {
  case tag::nil:       return builtin::type::nil;
  case tag::boolean:   return builtin::type::boolean;
  case tag::character: return builtin::type::character;
  case tag::integer:   return builtin::type::integer;
  case tag::symbol:    return builtin::type::symbol;
  default:             return get()->type;
  }
}

bool gc::operator==(managed_ptr lhs, managed_ptr rhs) noexcept
{
  return lhs.m_block == rhs.m_block && lhs.m_offset == rhs.m_offset
      && lhs.m_tag == rhs.m_tag     && lhs.m_flags == rhs.m_flags;
}

bool gc::operator!=(managed_ptr lhs, managed_ptr rhs) noexcept
{
  return !(lhs == rhs);
}

size_t std::hash<managed_ptr>::operator()(managed_ptr ptr) const
{
  const static std::hash<uint64_t> hasher{};
  return hasher(*reinterpret_cast<uint64_t*>(&ptr));
}
