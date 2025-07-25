// Generated by the protocol buffer compiler.  DO NOT EDIT!
// NO CHECKED-IN PROTOBUF GENCODE
// source: company.proto
// Protobuf C++ Version: 6.31.0

#include "company.pb.h"

#include <algorithm>
#include <type_traits>
#include "google/protobuf/io/coded_stream.h"
#include "google/protobuf/generated_message_tctable_impl.h"
#include "google/protobuf/extension_set.h"
#include "google/protobuf/generated_message_util.h"
#include "google/protobuf/wire_format_lite.h"
#include "google/protobuf/descriptor.h"
#include "google/protobuf/generated_message_reflection.h"
#include "google/protobuf/reflection_ops.h"
#include "google/protobuf/wire_format.h"
// @@protoc_insertion_point(includes)

// Must be included last.
#include "google/protobuf/port_def.inc"
PROTOBUF_PRAGMA_INIT_SEG
namespace _pb = ::google::protobuf;
namespace _pbi = ::google::protobuf::internal;
namespace _fl = ::google::protobuf::internal::field_layout;

inline constexpr Company::Impl_::Impl_(
    ::_pbi::ConstantInitialized) noexcept
      : _cached_size_{0},
        name_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        legal_address_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        occupation_(
            &::google::protobuf::internal::fixed_address_empty_string,
            ::_pbi::ConstantInitialized()),
        establishment_year_{0},
        foreign_trade_{false} {}

template <typename>
PROTOBUF_CONSTEXPR Company::Company(::_pbi::ConstantInitialized)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(Company_class_data_.base()),
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(),
#endif  // PROTOBUF_CUSTOM_VTABLE
      _impl_(::_pbi::ConstantInitialized()) {
}
struct CompanyDefaultTypeInternal {
  PROTOBUF_CONSTEXPR CompanyDefaultTypeInternal() : _instance(::_pbi::ConstantInitialized{}) {}
  ~CompanyDefaultTypeInternal() {}
  union {
    Company _instance;
  };
};

PROTOBUF_ATTRIBUTE_NO_DESTROY PROTOBUF_CONSTINIT
    PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 CompanyDefaultTypeInternal _Company_default_instance_;
static constexpr const ::_pb::EnumDescriptor *PROTOBUF_NONNULL *PROTOBUF_NULLABLE
    file_level_enum_descriptors_company_2eproto = nullptr;
static constexpr const ::_pb::ServiceDescriptor *PROTOBUF_NONNULL *PROTOBUF_NULLABLE
    file_level_service_descriptors_company_2eproto = nullptr;
const ::uint32_t
    TableStruct_company_2eproto::offsets[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
        protodesc_cold) = {
        0x081, // bitmap
        PROTOBUF_FIELD_OFFSET(::Company, _impl_._has_bits_),
        8, // hasbit index offset
        PROTOBUF_FIELD_OFFSET(::Company, _impl_.name_),
        PROTOBUF_FIELD_OFFSET(::Company, _impl_.legal_address_),
        PROTOBUF_FIELD_OFFSET(::Company, _impl_.establishment_year_),
        PROTOBUF_FIELD_OFFSET(::Company, _impl_.occupation_),
        PROTOBUF_FIELD_OFFSET(::Company, _impl_.foreign_trade_),
        0,
        1,
        3,
        2,
        4,
};

static const ::_pbi::MigrationSchema
    schemas[] ABSL_ATTRIBUTE_SECTION_VARIABLE(protodesc_cold) = {
        {0, sizeof(::Company)},
};
static const ::_pb::Message* PROTOBUF_NONNULL const file_default_instances[] = {
    &::_Company_default_instance_._instance,
};
const char descriptor_table_protodef_company_2eproto[] ABSL_ATTRIBUTE_SECTION_VARIABLE(
    protodesc_cold) = {
    "\n\rcompany.proto\"u\n\007Company\022\014\n\004name\030\001 \002(\t"
    "\022\025\n\rlegal_address\030\002 \002(\t\022\032\n\022establishment"
    "_year\030\003 \002(\005\022\022\n\noccupation\030\004 \001(\t\022\025\n\rforei"
    "gn_trade\030\005 \001(\010"
};
static ::absl::once_flag descriptor_table_company_2eproto_once;
PROTOBUF_CONSTINIT const ::_pbi::DescriptorTable descriptor_table_company_2eproto = {
    false,
    false,
    134,
    descriptor_table_protodef_company_2eproto,
    "company.proto",
    &descriptor_table_company_2eproto_once,
    nullptr,
    0,
    1,
    schemas,
    file_default_instances,
    TableStruct_company_2eproto::offsets,
    file_level_enum_descriptors_company_2eproto,
    file_level_service_descriptors_company_2eproto,
};
// ===================================================================

class Company::_Internal {
 public:
  using HasBits =
      decltype(::std::declval<Company>()._impl_._has_bits_);
  static constexpr ::int32_t kHasBitsOffset =
      8 * PROTOBUF_FIELD_OFFSET(Company, _impl_._has_bits_);
  static bool MissingRequiredFields(const HasBits& has_bits) {
    return ((has_bits[0] & 0x0000000b) ^ 0x0000000b) != 0;
  }
};

Company::Company(::google::protobuf::Arena* PROTOBUF_NULLABLE arena)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, Company_class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  SharedCtor(arena);
  // @@protoc_insertion_point(arena_constructor:Company)
}
PROTOBUF_NDEBUG_INLINE Company::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* PROTOBUF_NULLABLE arena, const Impl_& from,
    const ::Company& from_msg)
      : _has_bits_{from._has_bits_},
        _cached_size_{0},
        name_(arena, from.name_),
        legal_address_(arena, from.legal_address_),
        occupation_(arena, from.occupation_) {}

Company::Company(
    ::google::protobuf::Arena* PROTOBUF_NULLABLE arena,
    const Company& from)
#if defined(PROTOBUF_CUSTOM_VTABLE)
    : ::google::protobuf::Message(arena, Company_class_data_.base()) {
#else   // PROTOBUF_CUSTOM_VTABLE
    : ::google::protobuf::Message(arena) {
#endif  // PROTOBUF_CUSTOM_VTABLE
  Company* const _this = this;
  (void)_this;
  _internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(
      from._internal_metadata_);
  new (&_impl_) Impl_(internal_visibility(), arena, from._impl_, from);
  ::memcpy(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, establishment_year_),
           reinterpret_cast<const char *>(&from._impl_) +
               offsetof(Impl_, establishment_year_),
           offsetof(Impl_, foreign_trade_) -
               offsetof(Impl_, establishment_year_) +
               sizeof(Impl_::foreign_trade_));

  // @@protoc_insertion_point(copy_constructor:Company)
}
PROTOBUF_NDEBUG_INLINE Company::Impl_::Impl_(
    ::google::protobuf::internal::InternalVisibility visibility,
    ::google::protobuf::Arena* PROTOBUF_NULLABLE arena)
      : _cached_size_{0},
        name_(arena),
        legal_address_(arena),
        occupation_(arena) {}

inline void Company::SharedCtor(::_pb::Arena* PROTOBUF_NULLABLE arena) {
  new (&_impl_) Impl_(internal_visibility(), arena);
  ::memset(reinterpret_cast<char *>(&_impl_) +
               offsetof(Impl_, establishment_year_),
           0,
           offsetof(Impl_, foreign_trade_) -
               offsetof(Impl_, establishment_year_) +
               sizeof(Impl_::foreign_trade_));
}
Company::~Company() {
  // @@protoc_insertion_point(destructor:Company)
  SharedDtor(*this);
}
inline void Company::SharedDtor(MessageLite& self) {
  Company& this_ = static_cast<Company&>(self);
  this_._internal_metadata_.Delete<::google::protobuf::UnknownFieldSet>();
  ABSL_DCHECK(this_.GetArena() == nullptr);
  this_._impl_.name_.Destroy();
  this_._impl_.legal_address_.Destroy();
  this_._impl_.occupation_.Destroy();
  this_._impl_.~Impl_();
}

inline void* PROTOBUF_NONNULL Company::PlacementNew_(
    const void* PROTOBUF_NONNULL, void* PROTOBUF_NONNULL mem,
    ::google::protobuf::Arena* PROTOBUF_NULLABLE arena) {
  return ::new (mem) Company(arena);
}
constexpr auto Company::InternalNewImpl_() {
  return ::google::protobuf::internal::MessageCreator::CopyInit(sizeof(Company),
                                            alignof(Company));
}
constexpr auto Company::InternalGenerateClassData_() {
  return ::google::protobuf::internal::ClassDataFull{
      ::google::protobuf::internal::ClassData{
          &_Company_default_instance_._instance,
          &_table_.header,
          nullptr,  // OnDemandRegisterArenaDtor
          Company::IsInitializedImpl,
          &Company::MergeImpl,
          ::google::protobuf::Message::GetNewImpl<Company>(),
#if defined(PROTOBUF_CUSTOM_VTABLE)
          &Company::SharedDtor,
          ::google::protobuf::Message::GetClearImpl<Company>(), &Company::ByteSizeLong,
              &Company::_InternalSerialize,
#endif  // PROTOBUF_CUSTOM_VTABLE
          PROTOBUF_FIELD_OFFSET(Company, _impl_._cached_size_),
          false,
      },
      &Company::kDescriptorMethods,
      &descriptor_table_company_2eproto,
      nullptr,  // tracker
  };
}

PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1 const
    ::google::protobuf::internal::ClassDataFull Company_class_data_ =
        Company::InternalGenerateClassData_();

PROTOBUF_ATTRIBUTE_WEAK const ::google::protobuf::internal::ClassData* PROTOBUF_NONNULL
Company::GetClassData() const {
  ::google::protobuf::internal::PrefetchToLocalCache(&Company_class_data_);
  ::google::protobuf::internal::PrefetchToLocalCache(Company_class_data_.tc_table);
  return Company_class_data_.base();
}
PROTOBUF_CONSTINIT PROTOBUF_ATTRIBUTE_INIT_PRIORITY1
const ::_pbi::TcParseTable<3, 5, 0, 43, 2>
Company::_table_ = {
  {
    PROTOBUF_FIELD_OFFSET(Company, _impl_._has_bits_),
    0, // no _extensions_
    5, 56,  // max_field_number, fast_idx_mask
    offsetof(decltype(_table_), field_lookup_table),
    4294967264,  // skipmap
    offsetof(decltype(_table_), field_entries),
    5,  // num_field_entries
    0,  // num_aux_entries
    offsetof(decltype(_table_), field_names),  // no aux_entries
    Company_class_data_.base(),
    nullptr,  // post_loop_handler
    ::_pbi::TcParser::GenericFallback,  // fallback
    #ifdef PROTOBUF_PREFETCH_PARSE_TABLE
    ::_pbi::TcParser::GetTable<::Company>(),  // to_prefetch
    #endif  // PROTOBUF_PREFETCH_PARSE_TABLE
  }, {{
    {::_pbi::TcParser::MiniParse, {}},
    // required string name = 1;
    {::_pbi::TcParser::FastSS1,
     {10, 0, 0, PROTOBUF_FIELD_OFFSET(Company, _impl_.name_)}},
    // required string legal_address = 2;
    {::_pbi::TcParser::FastSS1,
     {18, 1, 0, PROTOBUF_FIELD_OFFSET(Company, _impl_.legal_address_)}},
    // required int32 establishment_year = 3;
    {::_pbi::TcParser::SingularVarintNoZag1<::uint32_t, offsetof(Company, _impl_.establishment_year_), 3>(),
     {24, 3, 0, PROTOBUF_FIELD_OFFSET(Company, _impl_.establishment_year_)}},
    // optional string occupation = 4;
    {::_pbi::TcParser::FastSS1,
     {34, 2, 0, PROTOBUF_FIELD_OFFSET(Company, _impl_.occupation_)}},
    // optional bool foreign_trade = 5;
    {::_pbi::TcParser::SingularVarintNoZag1<bool, offsetof(Company, _impl_.foreign_trade_), 4>(),
     {40, 4, 0, PROTOBUF_FIELD_OFFSET(Company, _impl_.foreign_trade_)}},
    {::_pbi::TcParser::MiniParse, {}},
    {::_pbi::TcParser::MiniParse, {}},
  }}, {{
    65535, 65535
  }}, {{
    // required string name = 1;
    {PROTOBUF_FIELD_OFFSET(Company, _impl_.name_), _Internal::kHasBitsOffset + 0, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kRawString | ::_fl::kRepAString)},
    // required string legal_address = 2;
    {PROTOBUF_FIELD_OFFSET(Company, _impl_.legal_address_), _Internal::kHasBitsOffset + 1, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kRawString | ::_fl::kRepAString)},
    // required int32 establishment_year = 3;
    {PROTOBUF_FIELD_OFFSET(Company, _impl_.establishment_year_), _Internal::kHasBitsOffset + 3, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kInt32)},
    // optional string occupation = 4;
    {PROTOBUF_FIELD_OFFSET(Company, _impl_.occupation_), _Internal::kHasBitsOffset + 2, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kRawString | ::_fl::kRepAString)},
    // optional bool foreign_trade = 5;
    {PROTOBUF_FIELD_OFFSET(Company, _impl_.foreign_trade_), _Internal::kHasBitsOffset + 4, 0,
    (0 | ::_fl::kFcOptional | ::_fl::kBool)},
  }},
  // no aux_entries
  {{
    "\7\4\15\0\12\0\0\0"
    "Company"
    "name"
    "legal_address"
    "occupation"
  }},
};
PROTOBUF_NOINLINE void Company::Clear() {
// @@protoc_insertion_point(message_clear_start:Company)
  ::google::protobuf::internal::TSanWrite(&_impl_);
  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void) cached_has_bits;

  cached_has_bits = _impl_._has_bits_[0];
  if ((cached_has_bits & 0x00000007u) != 0) {
    if ((cached_has_bits & 0x00000001u) != 0) {
      _impl_.name_.ClearNonDefaultToEmpty();
    }
    if ((cached_has_bits & 0x00000002u) != 0) {
      _impl_.legal_address_.ClearNonDefaultToEmpty();
    }
    if ((cached_has_bits & 0x00000004u) != 0) {
      _impl_.occupation_.ClearNonDefaultToEmpty();
    }
  }
  if ((cached_has_bits & 0x00000018u) != 0) {
    ::memset(&_impl_.establishment_year_, 0, static_cast<::size_t>(
        reinterpret_cast<char*>(&_impl_.foreign_trade_) -
        reinterpret_cast<char*>(&_impl_.establishment_year_)) + sizeof(_impl_.foreign_trade_));
  }
  _impl_._has_bits_.Clear();
  _internal_metadata_.Clear<::google::protobuf::UnknownFieldSet>();
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
::uint8_t* PROTOBUF_NONNULL Company::_InternalSerialize(
    const ::google::protobuf::MessageLite& base, ::uint8_t* PROTOBUF_NONNULL target,
    ::google::protobuf::io::EpsCopyOutputStream* PROTOBUF_NONNULL stream) {
  const Company& this_ = static_cast<const Company&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
::uint8_t* PROTOBUF_NONNULL Company::_InternalSerialize(
    ::uint8_t* PROTOBUF_NONNULL target,
    ::google::protobuf::io::EpsCopyOutputStream* PROTOBUF_NONNULL stream) const {
  const Company& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
  // @@protoc_insertion_point(serialize_to_array_start:Company)
  ::uint32_t cached_has_bits = 0;
  (void)cached_has_bits;

  cached_has_bits = this_._impl_._has_bits_[0];
  // required string name = 1;
  if ((cached_has_bits & 0x00000001u) != 0) {
    const ::std::string& _s = this_._internal_name();
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(_s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormat::SERIALIZE,
                                "Company.name");
    target = stream->WriteStringMaybeAliased(1, _s, target);
  }

  // required string legal_address = 2;
  if ((cached_has_bits & 0x00000002u) != 0) {
    const ::std::string& _s = this_._internal_legal_address();
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(_s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormat::SERIALIZE,
                                "Company.legal_address");
    target = stream->WriteStringMaybeAliased(2, _s, target);
  }

  // required int32 establishment_year = 3;
  if ((cached_has_bits & 0x00000008u) != 0) {
    target =
        ::google::protobuf::internal::WireFormatLite::WriteInt32ToArrayWithField<3>(
            stream, this_._internal_establishment_year(), target);
  }

  // optional string occupation = 4;
  if ((cached_has_bits & 0x00000004u) != 0) {
    const ::std::string& _s = this_._internal_occupation();
    ::google::protobuf::internal::WireFormat::VerifyUTF8StringNamedField(_s.data(), static_cast<int>(_s.length()), ::google::protobuf::internal::WireFormat::SERIALIZE,
                                "Company.occupation");
    target = stream->WriteStringMaybeAliased(4, _s, target);
  }

  // optional bool foreign_trade = 5;
  if ((cached_has_bits & 0x00000010u) != 0) {
    target = stream->EnsureSpace(target);
    target = ::_pbi::WireFormatLite::WriteBoolToArray(
        5, this_._internal_foreign_trade(), target);
  }

  if (ABSL_PREDICT_FALSE(this_._internal_metadata_.have_unknown_fields())) {
    target =
        ::_pbi::WireFormat::InternalSerializeUnknownFieldsToArray(
            this_._internal_metadata_.unknown_fields<::google::protobuf::UnknownFieldSet>(::google::protobuf::UnknownFieldSet::default_instance), target, stream);
  }
  // @@protoc_insertion_point(serialize_to_array_end:Company)
  return target;
}

#if defined(PROTOBUF_CUSTOM_VTABLE)
::size_t Company::ByteSizeLong(const MessageLite& base) {
  const Company& this_ = static_cast<const Company&>(base);
#else   // PROTOBUF_CUSTOM_VTABLE
::size_t Company::ByteSizeLong() const {
  const Company& this_ = *this;
#endif  // PROTOBUF_CUSTOM_VTABLE
  // @@protoc_insertion_point(message_byte_size_start:Company)
  ::size_t total_size = 0;

  ::uint32_t cached_has_bits = 0;
  // Prevent compiler warnings about cached_has_bits being unused
  (void)cached_has_bits;

  ::_pbi::Prefetch5LinesFrom7Lines(&this_);
  cached_has_bits = this_._impl_._has_bits_[0];
  total_size += static_cast<bool>(0x00000010u & cached_has_bits) * 2;
  if ((cached_has_bits & 0x00000003u) != 0) {
    // required string name = 1;
    if ((cached_has_bits & 0x00000001u) != 0) {
      total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                      this_._internal_name());
    }
    // required string legal_address = 2;
    if ((cached_has_bits & 0x00000002u) != 0) {
      total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                      this_._internal_legal_address());
    }
  }
   {
    // optional string occupation = 4;
    if ((cached_has_bits & 0x00000004u) != 0) {
      total_size += 1 + ::google::protobuf::internal::WireFormatLite::StringSize(
                                      this_._internal_occupation());
    }
  }
   {
    // required int32 establishment_year = 3;
    if ((cached_has_bits & 0x00000008u) != 0) {
      total_size += ::_pbi::WireFormatLite::Int32SizePlusOne(
          this_._internal_establishment_year());
    }
  }
  return this_.MaybeComputeUnknownFieldsSize(total_size,
                                             &this_._impl_._cached_size_);
}

void Company::MergeImpl(::google::protobuf::MessageLite& to_msg, const ::google::protobuf::MessageLite& from_msg) {
  auto* const _this = static_cast<Company*>(&to_msg);
  auto& from = static_cast<const Company&>(from_msg);
  // @@protoc_insertion_point(class_specific_merge_from_start:Company)
  ABSL_DCHECK_NE(&from, _this);
  ::uint32_t cached_has_bits = 0;
  (void) cached_has_bits;

  cached_has_bits = from._impl_._has_bits_[0];
  if ((cached_has_bits & 0x0000001fu) != 0) {
    if ((cached_has_bits & 0x00000001u) != 0) {
      _this->_internal_set_name(from._internal_name());
    }
    if ((cached_has_bits & 0x00000002u) != 0) {
      _this->_internal_set_legal_address(from._internal_legal_address());
    }
    if ((cached_has_bits & 0x00000004u) != 0) {
      _this->_internal_set_occupation(from._internal_occupation());
    }
    if ((cached_has_bits & 0x00000008u) != 0) {
      _this->_impl_.establishment_year_ = from._impl_.establishment_year_;
    }
    if ((cached_has_bits & 0x00000010u) != 0) {
      _this->_impl_.foreign_trade_ = from._impl_.foreign_trade_;
    }
  }
  _this->_impl_._has_bits_[0] |= cached_has_bits;
  _this->_internal_metadata_.MergeFrom<::google::protobuf::UnknownFieldSet>(from._internal_metadata_);
}

void Company::CopyFrom(const Company& from) {
// @@protoc_insertion_point(class_specific_copy_from_start:Company)
  if (&from == this) return;
  Clear();
  MergeFrom(from);
}

PROTOBUF_NOINLINE bool Company::IsInitializedImpl(
    const MessageLite& msg) {
  auto& this_ = static_cast<const Company&>(msg);
  if (_Internal::MissingRequiredFields(this_._impl_._has_bits_)) {
    return false;
  }
  return true;
}

void Company::InternalSwap(Company* PROTOBUF_RESTRICT PROTOBUF_NONNULL other) {
  using ::std::swap;
  auto* arena = GetArena();
  ABSL_DCHECK_EQ(arena, other->GetArena());
  _internal_metadata_.InternalSwap(&other->_internal_metadata_);
  swap(_impl_._has_bits_[0], other->_impl_._has_bits_[0]);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.name_, &other->_impl_.name_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.legal_address_, &other->_impl_.legal_address_, arena);
  ::_pbi::ArenaStringPtr::InternalSwap(&_impl_.occupation_, &other->_impl_.occupation_, arena);
  ::google::protobuf::internal::memswap<
      PROTOBUF_FIELD_OFFSET(Company, _impl_.foreign_trade_)
      + sizeof(Company::_impl_.foreign_trade_)
      - PROTOBUF_FIELD_OFFSET(Company, _impl_.establishment_year_)>(
          reinterpret_cast<char*>(&_impl_.establishment_year_),
          reinterpret_cast<char*>(&other->_impl_.establishment_year_));
}

::google::protobuf::Metadata Company::GetMetadata() const {
  return ::google::protobuf::Message::GetMetadataImpl(GetClassData()->full());
}
// @@protoc_insertion_point(namespace_scope)
namespace google {
namespace protobuf {
}  // namespace protobuf
}  // namespace google
// @@protoc_insertion_point(global_scope)
PROTOBUF_ATTRIBUTE_INIT_PRIORITY2 static ::std::false_type
    _static_init2_ [[maybe_unused]] =
        (::_pbi::AddDescriptors(&descriptor_table_company_2eproto),
         ::std::false_type{});
#include "google/protobuf/port_undef.inc"
