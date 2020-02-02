#ifndef ONEFLOW_CORE_COMMON_OBJECT_MSG_LIST_H_
#define ONEFLOW_CORE_COMMON_OBJECT_MSG_LIST_H_

#include "oneflow/core/common/object_msg_core.h"
#include "oneflow/core/common/embedded_list.h"

namespace oneflow {

#define OBJECT_MSG_DEFINE_LIST_ITEM(field_name)                                     \
  static_assert(__is_object_message_type__, "this struct is not a object message"); \
  _OBJECT_MSG_DEFINE_LIST_ITEM(DSS_GET_FIELD_COUNTER(), field_name);

#define OBJECT_MSG_DEFINE_LIST_HEAD(elem_type, elem_field_name, field_name)         \
  static_assert(__is_object_message_type__, "this struct is not a object message"); \
  _OBJECT_MSG_DEFINE_LIST_HEAD(DSS_GET_FIELD_COUNTER(), elem_type, elem_field_name, field_name);

#define OBJECT_MSG_LIST_FOR_EACH(list_ptr, item)                                \
  for (auto item = (list_ptr)->Begin(), __next_item__ = (list_ptr)->Next(item); \
       !(list_ptr)->EqualsEnd(item);                                            \
       item = __next_item__, __next_item__ = (list_ptr)->Next(__next_item__))

#define OBJECT_MSG_LIST_FOR_EACH_UNSAFE(list_ptr, item) \
  for (auto item = (list_ptr)->Begin(); !(list_ptr)->EqualsEnd(item); item = (list_ptr)->Next(item))

// details

#define _OBJECT_MSG_DEFINE_LIST_HEAD(field_counter, elem_type, elem_field_name, field_name) \
  _OBJECT_MSG_DEFINE_LIST_HEAD_FIELD(elem_type, elem_field_name, field_name)                \
  OBJECT_MSG_OVERLOAD_INIT(field_counter, ObjectMsgEmbeddedListHeadInit);                   \
  OBJECT_MSG_OVERLOAD_DELETE(field_counter, ObjectMsgEmbeddedListHeadDelete);               \
  DSS_DEFINE_FIELD(field_counter, "object message", OF_PP_CAT(field_name, _));

#define _OBJECT_MSG_DEFINE_LIST_HEAD_FIELD(elem_type, elem_field_name, field_name)             \
 public:                                                                                       \
  using OF_PP_CAT(field_name, _ObjectMsgListType) = TrivialObjectMsgList<                      \
      StructField<OBJECT_MSG_TYPE(elem_type), EmbeddedListItem,                                \
                  OBJECT_MSG_TYPE(elem_type)::OF_PP_CAT(elem_field_name, _DssFieldOffset)()>>; \
  const OF_PP_CAT(field_name, _ObjectMsgListType) & field_name() const {                       \
    return OF_PP_CAT(field_name, _);                                                           \
  }                                                                                            \
  OF_PP_CAT(field_name, _ObjectMsgListType) * OF_PP_CAT(mut_, field_name)() {                  \
    return &OF_PP_CAT(field_name, _);                                                          \
  }                                                                                            \
  OF_PP_CAT(field_name, _ObjectMsgListType) * OF_PP_CAT(mutable_, field_name)() {              \
    return &OF_PP_CAT(field_name, _);                                                          \
  }                                                                                            \
                                                                                               \
 private:                                                                                      \
  OF_PP_CAT(field_name, _ObjectMsgListType) OF_PP_CAT(field_name, _);

#define _OBJECT_MSG_DEFINE_LIST_ITEM(field_counter, field_name)               \
  _OBJECT_MSG_DEFINE_LIST_ITEM_FIELD(field_name)                              \
  OBJECT_MSG_OVERLOAD_INIT(field_counter, ObjectMsgEmbeddedListItemInit);     \
  OBJECT_MSG_OVERLOAD_DELETE(field_counter, ObjectMsgEmbeddedListItemDelete); \
  DSS_DEFINE_FIELD(field_counter, "object message", OF_PP_CAT(field_name, _));

#define _OBJECT_MSG_DEFINE_LIST_ITEM_FIELD(field_name) \
 private:                                              \
  EmbeddedListItem OF_PP_CAT(field_name, _);

#define OBJECT_MSG_LIST(obj_msg_type, obj_msg_field)               \
  ObjectMsgList<                                                   \
      StructField<OBJECT_MSG_TYPE(obj_msg_type), EmbeddedListItem, \
                  OBJECT_MSG_TYPE(obj_msg_type)::OF_PP_CAT(obj_msg_field, _DssFieldOffset)()>>

template<typename WalkCtxType, typename PtrFieldType>
struct ObjectMsgEmbeddedListHeadInit {
  static void Call(WalkCtxType* ctx, PtrFieldType* field, const char* field_name) {
    field->__Init__();
  }
};

template<typename WalkCtxType, typename PtrFieldType>
struct ObjectMsgEmbeddedListHeadDelete {
  static void Call(WalkCtxType* ctx, PtrFieldType* field, const char* field_name) {
    field->Clear();
  }
};

template<typename WalkCtxType, typename PtrFieldType>
struct ObjectMsgEmbeddedListItemInit {
  static void Call(WalkCtxType* ctx, EmbeddedListItem* field, const char* field_name) {
    field->__Init__();
  }
};

template<typename WalkCtxType, typename PtrFieldType>
struct ObjectMsgEmbeddedListItemDelete {
  static void Call(WalkCtxType* ctx, EmbeddedListItem* field, const char* field_name) {
    CHECK(field->empty());
  }
};

template<typename ItemField>
class TrivialObjectMsgList {
 public:
  using item_type = typename ItemField::struct_type;

  std::size_t size() const { return list_head_.size(); }
  bool empty() const { return list_head_.empty(); }

  void __Init__() { list_head_.__Init__(); }

  void GetBegin(ObjectMsgPtr<item_type>* begin) {
    if (list_head_.empty()) { return begin->Reset(); }
    begin->Reset(list_head_.begin_item());
  }
  void ResetToNext(ObjectMsgPtr<item_type>* ptr) {
    if (!*ptr) { return; }
    auto* next_item = list_head_.next_item(ptr->Mutable());
    if (next_item == list_head_.end_item()) { next_item = nullptr; }
    ptr->Reset(next_item);
  }
  ObjectMsgPtr<item_type> Begin() {
    ObjectMsgPtr<item_type> begin;
    GetBegin(&begin);
    return begin;
  }
  ObjectMsgPtr<item_type> Next(const ObjectMsgPtr<item_type>& ptr) {
    ObjectMsgPtr<item_type> ret(ptr);
    ResetToNext(&ret);
    return ret;
  }

  void GetLast(ObjectMsgPtr<item_type>* last) {
    if (list_head_.empty()) { return last->Reset(); }
    last->Reset(list_head_.last_item());
  }
  ObjectMsgPtr<item_type> Last() {
    ObjectMsgPtr<item_type> begin;
    GetLast(&begin);
    return begin;
  }
  void GetBegin(ObjectMsgPtr<item_type>* begin, ObjectMsgPtr<item_type>* next) {
    GetBegin(begin);
    GetBegin(next);
    ResetToNext(next);
  }
  void ResetToNext(ObjectMsgPtr<item_type>* ptr, ObjectMsgPtr<item_type>* next) {
    *ptr = *next;
    ResetToNext(next);
  }

  bool EqualsEnd(const ObjectMsgPtr<item_type>& ptr) const { return !ptr; }

  void MoveToDstBack(item_type* ptr, TrivialObjectMsgList* dst) {
    list_head_.MoveToDstBack(ptr, &dst->list_head_);
  }
  void MoveToDstFront(item_type* ptr, TrivialObjectMsgList* dst) {
    list_head_.MoveToDstFront(ptr, &dst->list_head_);
  }
  item_type* MoveFrontToDstBack(TrivialObjectMsgList* dst) {
    item_type* begin = list_head_.begin_item();
    MoveToDstBack(begin, dst);
    return begin;
  }

  void PushBack(item_type* ptr) {
    list_head_.PushBack(ptr);
    ObjectMsgPtrUtil::Ref(ptr);
  }

  void PushFront(item_type* ptr) {
    list_head_.PushFront(ptr);
    ObjectMsgPtrUtil::Ref(ptr);
  }

  void Erase(item_type* ptr) {
    list_head_.Erase(ptr);
    ObjectMsgPtrUtil::ReleaseRef(ptr);
  }

  ObjectMsgPtr<item_type> PopBack() {
    ObjectMsgPtr<item_type> ptr;
    if (list_head_.empty()) { return ptr; }
    ptr.Reset(list_head_.PopBack());
    ObjectMsgPtrUtil::ReleaseRef(ptr.Mutable());
    return ptr;
  }

  ObjectMsgPtr<item_type> PopFront() {
    ObjectMsgPtr<item_type> ptr;
    if (list_head_.empty()) { return ptr; }
    ptr.Reset(list_head_.PopFront());
    ObjectMsgPtrUtil::ReleaseRef(ptr.Mutable());
    return ptr;
  }

  void MoveTo(TrivialObjectMsgList* list) { MoveToDstBack(list); }
  void MoveToDstBack(TrivialObjectMsgList* list) { list_head_.MoveToDstBack(&list->list_head_); }

  void Clear() {
    while (!empty()) { ObjectMsgPtrUtil::ReleaseRef(list_head_.PopFront()); }
  }

 private:
  EmbeddedListHead<ItemField> list_head_;
};

template<typename ItemField>
class ObjectMsgList : public TrivialObjectMsgList<ItemField> {
 public:
  ObjectMsgList(const ObjectMsgList&) = delete;
  ObjectMsgList(ObjectMsgList&&) = delete;
  ObjectMsgList() { this->__Init__(); }
  ~ObjectMsgList() { this->Clear(); }
};
}

#endif  // ONEFLOW_CORE_COMMON_OBJECT_MSG_LIST_H_