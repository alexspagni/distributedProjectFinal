//
// Generated file, do not edit! Created by opp_msgtool 6.0 from keysValueMessage.msg.
//

// Disable warnings about unused variables, empty switch stmts, etc:
#ifdef _MSC_VER
#  pragma warning(disable:4101)
#  pragma warning(disable:4065)
#endif

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wshadow"
#  pragma clang diagnostic ignored "-Wconversion"
#  pragma clang diagnostic ignored "-Wunused-parameter"
#  pragma clang diagnostic ignored "-Wc++98-compat"
#  pragma clang diagnostic ignored "-Wunreachable-code-break"
#  pragma clang diagnostic ignored "-Wold-style-cast"
#elif defined(__GNUC__)
#  pragma GCC diagnostic ignored "-Wshadow"
#  pragma GCC diagnostic ignored "-Wconversion"
#  pragma GCC diagnostic ignored "-Wunused-parameter"
#  pragma GCC diagnostic ignored "-Wold-style-cast"
#  pragma GCC diagnostic ignored "-Wsuggest-attribute=noreturn"
#  pragma GCC diagnostic ignored "-Wfloat-conversion"
#endif

#include <iostream>
#include <sstream>
#include <memory>
#include <type_traits>
#include "keysValueMessage_m.h"

namespace omnetpp {

// Template pack/unpack rules. They are declared *after* a1l type-specific pack functions for multiple reasons.
// They are in the omnetpp namespace, to allow them to be found by argument-dependent lookup via the cCommBuffer argument

// Packing/unpacking an std::vector
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::vector<T,A>& v)
{
    int n = v.size();
    doParsimPacking(buffer, n);
    for (int i = 0; i < n; i++)
        doParsimPacking(buffer, v[i]);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::vector<T,A>& v)
{
    int n;
    doParsimUnpacking(buffer, n);
    v.resize(n);
    for (int i = 0; i < n; i++)
        doParsimUnpacking(buffer, v[i]);
}

// Packing/unpacking an std::list
template<typename T, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::list<T,A>& l)
{
    doParsimPacking(buffer, (int)l.size());
    for (typename std::list<T,A>::const_iterator it = l.begin(); it != l.end(); ++it)
        doParsimPacking(buffer, (T&)*it);
}

template<typename T, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::list<T,A>& l)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        l.push_back(T());
        doParsimUnpacking(buffer, l.back());
    }
}

// Packing/unpacking an std::set
template<typename T, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::set<T,Tr,A>& s)
{
    doParsimPacking(buffer, (int)s.size());
    for (typename std::set<T,Tr,A>::const_iterator it = s.begin(); it != s.end(); ++it)
        doParsimPacking(buffer, *it);
}

template<typename T, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::set<T,Tr,A>& s)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        T x;
        doParsimUnpacking(buffer, x);
        s.insert(x);
    }
}

// Packing/unpacking an std::map
template<typename K, typename V, typename Tr, typename A>
void doParsimPacking(omnetpp::cCommBuffer *buffer, const std::map<K,V,Tr,A>& m)
{
    doParsimPacking(buffer, (int)m.size());
    for (typename std::map<K,V,Tr,A>::const_iterator it = m.begin(); it != m.end(); ++it) {
        doParsimPacking(buffer, it->first);
        doParsimPacking(buffer, it->second);
    }
}

template<typename K, typename V, typename Tr, typename A>
void doParsimUnpacking(omnetpp::cCommBuffer *buffer, std::map<K,V,Tr,A>& m)
{
    int n;
    doParsimUnpacking(buffer, n);
    for (int i = 0; i < n; i++) {
        K k; V v;
        doParsimUnpacking(buffer, k);
        doParsimUnpacking(buffer, v);
        m[k] = v;
    }
}

// Default pack/unpack function for arrays
template<typename T>
void doParsimArrayPacking(omnetpp::cCommBuffer *b, const T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimPacking(b, t[i]);
}

template<typename T>
void doParsimArrayUnpacking(omnetpp::cCommBuffer *b, T *t, int n)
{
    for (int i = 0; i < n; i++)
        doParsimUnpacking(b, t[i]);
}

// Default rule to prevent compiler from choosing base class' doParsimPacking() function
template<typename T>
void doParsimPacking(omnetpp::cCommBuffer *, const T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimPacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

template<typename T>
void doParsimUnpacking(omnetpp::cCommBuffer *, T& t)
{
    throw omnetpp::cRuntimeError("Parsim error: No doParsimUnpacking() function for type %s", omnetpp::opp_typename(typeid(t)));
}

}  // namespace omnetpp

Register_Class(KeysValueMessage)

KeysValueMessage::KeysValueMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

KeysValueMessage::KeysValueMessage(const KeysValueMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

KeysValueMessage::~KeysValueMessage()
{
}

KeysValueMessage& KeysValueMessage::operator=(const KeysValueMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void KeysValueMessage::copy(const KeysValueMessage& other)
{
    this->reducerToSend = other.reducerToSend;
    this->key = other.key;
    for (size_t i = 0; i < 100; i++) {
        this->value[i] = other.value[i];
    }
}

void KeysValueMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    doParsimPacking(b,this->reducerToSend);
    doParsimPacking(b,this->key);
    doParsimArrayPacking(b,this->value,100);
}

void KeysValueMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    doParsimUnpacking(b,this->reducerToSend);
    doParsimUnpacking(b,this->key);
    doParsimArrayUnpacking(b,this->value,100);
}

int KeysValueMessage::getReducerToSend() const
{
    return this->reducerToSend;
}

void KeysValueMessage::setReducerToSend(int reducerToSend)
{
    this->reducerToSend = reducerToSend;
}

int KeysValueMessage::getKey() const
{
    return this->key;
}

void KeysValueMessage::setKey(int key)
{
    this->key = key;
}

size_t KeysValueMessage::getValueArraySize() const
{
    return 100;
}

int KeysValueMessage::getValue(size_t k) const
{
    if (k >= 100) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)100, (unsigned long)k);
    return this->value[k];
}

void KeysValueMessage::setValue(size_t k, int value)
{
    if (k >= 100) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)100, (unsigned long)k);
    this->value[k] = value;
}
void KeysValueMessage::setEntireArray(int *otherArray){
    for (int i=0;i<100;i++){
        this->value[i]=otherArray[i];
    }
}

class KeysValueMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_reducerToSend,
        FIELD_key,
        FIELD_value,
    };
  public:
    KeysValueMessageDescriptor();
    virtual ~KeysValueMessageDescriptor();

    virtual bool doesSupport(omnetpp::cObject *obj) const override;
    virtual const char **getPropertyNames() const override;
    virtual const char *getProperty(const char *propertyName) const override;
    virtual int getFieldCount() const override;
    virtual const char *getFieldName(int field) const override;
    virtual int findField(const char *fieldName) const override;
    virtual unsigned int getFieldTypeFlags(int field) const override;
    virtual const char *getFieldTypeString(int field) const override;
    virtual const char **getFieldPropertyNames(int field) const override;
    virtual const char *getFieldProperty(int field, const char *propertyName) const override;
    virtual int getFieldArraySize(omnetpp::any_ptr object, int field) const override;
    virtual void setFieldArraySize(omnetpp::any_ptr object, int field, int size) const override;

    virtual const char *getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const override;
    virtual std::string getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const override;
    virtual omnetpp::cValue getFieldValue(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const override;

    virtual const char *getFieldStructName(int field) const override;
    virtual omnetpp::any_ptr getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const override;
    virtual void setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const override;
};

Register_ClassDescriptor(KeysValueMessageDescriptor)

KeysValueMessageDescriptor::KeysValueMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(KeysValueMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

KeysValueMessageDescriptor::~KeysValueMessageDescriptor()
{
    delete[] propertyNames;
}

bool KeysValueMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<KeysValueMessage *>(obj)!=nullptr;
}

const char **KeysValueMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *KeysValueMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int KeysValueMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 3+base->getFieldCount() : 3;
}

unsigned int KeysValueMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISEDITABLE,    // FIELD_reducerToSend
        FD_ISEDITABLE,    // FIELD_key
        FD_ISARRAY | FD_ISEDITABLE,    // FIELD_value
    };
    return (field >= 0 && field < 3) ? fieldTypeFlags[field] : 0;
}

const char *KeysValueMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "reducerToSend",
        "key",
        "value",
    };
    return (field >= 0 && field < 3) ? fieldNames[field] : nullptr;
}

int KeysValueMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "reducerToSend") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "key") == 0) return baseIndex + 1;
    if (strcmp(fieldName, "value") == 0) return baseIndex + 2;
    return base ? base->findField(fieldName) : -1;
}

const char *KeysValueMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_reducerToSend
        "int",    // FIELD_key
        "int",    // FIELD_value
    };
    return (field >= 0 && field < 3) ? fieldTypeStrings[field] : nullptr;
}

const char **KeysValueMessageDescriptor::getFieldPropertyNames(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldPropertyNames(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

const char *KeysValueMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldProperty(field, propertyName);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    }
}

int KeysValueMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        case FIELD_value: return 100;
        default: return 0;
    }
}

void KeysValueMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'KeysValueMessage'", field);
    }
}

const char *KeysValueMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string KeysValueMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducerToSend: return long2string(pp->getReducerToSend());
        case FIELD_key: return long2string(pp->getKey());
        case FIELD_value: return long2string(pp->getValue(i));
        default: return "";
    }
}

void KeysValueMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducerToSend: pp->setReducerToSend(string2long(value)); break;
        case FIELD_key: pp->setKey(string2long(value)); break;
        case FIELD_value: pp->setValue(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'KeysValueMessage'", field);
    }
}

omnetpp::cValue KeysValueMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducerToSend: return pp->getReducerToSend();
        case FIELD_key: return pp->getKey();
        case FIELD_value: return pp->getValue(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'KeysValueMessage' as cValue -- field index out of range?", field);
    }
}

void KeysValueMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducerToSend: pp->setReducerToSend(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_key: pp->setKey(omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_value: pp->setValue(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'KeysValueMessage'", field);
    }
}

const char *KeysValueMessageDescriptor::getFieldStructName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructName(field);
        field -= base->getFieldCount();
    }
    switch (field) {
        default: return nullptr;
    };
}

omnetpp::any_ptr KeysValueMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void KeysValueMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    KeysValueMessage *pp = omnetpp::fromAnyPtr<KeysValueMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'KeysValueMessage'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

