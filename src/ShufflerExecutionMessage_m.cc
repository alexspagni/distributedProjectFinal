//
// Generated file, do not edit! Created by opp_msgtool 6.0 from ShufflerExecutionMessage.msg.
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
#include "ShufflerExecutionMessage_m.h"

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

Register_Class(ShufflerExecutionMessage)

ShufflerExecutionMessage::ShufflerExecutionMessage(const char *name, short kind) : ::omnetpp::cMessage(name, kind)
{
}

ShufflerExecutionMessage::ShufflerExecutionMessage(const ShufflerExecutionMessage& other) : ::omnetpp::cMessage(other)
{
    copy(other);
}

ShufflerExecutionMessage::~ShufflerExecutionMessage()
{
    delete [] this->reducersAvailable;
    delete [] this->partitionsElaborated;
}

ShufflerExecutionMessage& ShufflerExecutionMessage::operator=(const ShufflerExecutionMessage& other)
{
    if (this == &other) return *this;
    ::omnetpp::cMessage::operator=(other);
    copy(other);
    return *this;
}

void ShufflerExecutionMessage::copy(const ShufflerExecutionMessage& other)
{
    delete [] this->reducersAvailable;
    this->reducersAvailable = (other.reducersAvailable_arraysize==0) ? nullptr : new int[other.reducersAvailable_arraysize];
    reducersAvailable_arraysize = other.reducersAvailable_arraysize;
    for (size_t i = 0; i < reducersAvailable_arraysize; i++) {
        this->reducersAvailable[i] = other.reducersAvailable[i];
    }
    delete [] this->partitionsElaborated;
    this->partitionsElaborated = (other.partitionsElaborated_arraysize==0) ? nullptr : new int[other.partitionsElaborated_arraysize];
    partitionsElaborated_arraysize = other.partitionsElaborated_arraysize;
    for (size_t i = 0; i < partitionsElaborated_arraysize; i++) {
        this->partitionsElaborated[i] = other.partitionsElaborated[i];
    }
}

void ShufflerExecutionMessage::parsimPack(omnetpp::cCommBuffer *b) const
{
    ::omnetpp::cMessage::parsimPack(b);
    b->pack(reducersAvailable_arraysize);
    doParsimArrayPacking(b,this->reducersAvailable,reducersAvailable_arraysize);
    b->pack(partitionsElaborated_arraysize);
    doParsimArrayPacking(b,this->partitionsElaborated,partitionsElaborated_arraysize);
}

void ShufflerExecutionMessage::parsimUnpack(omnetpp::cCommBuffer *b)
{
    ::omnetpp::cMessage::parsimUnpack(b);
    delete [] this->reducersAvailable;
    b->unpack(reducersAvailable_arraysize);
    if (reducersAvailable_arraysize == 0) {
        this->reducersAvailable = nullptr;
    } else {
        this->reducersAvailable = new int[reducersAvailable_arraysize];
        doParsimArrayUnpacking(b,this->reducersAvailable,reducersAvailable_arraysize);
    }
    delete [] this->partitionsElaborated;
    b->unpack(partitionsElaborated_arraysize);
    if (partitionsElaborated_arraysize == 0) {
        this->partitionsElaborated = nullptr;
    } else {
        this->partitionsElaborated = new int[partitionsElaborated_arraysize];
        doParsimArrayUnpacking(b,this->partitionsElaborated,partitionsElaborated_arraysize);
    }
}

size_t ShufflerExecutionMessage::getReducersAvailableArraySize() const
{
    return reducersAvailable_arraysize;
}

int ShufflerExecutionMessage::getReducersAvailable(size_t k) const
{
    if (k >= reducersAvailable_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)reducersAvailable_arraysize, (unsigned long)k);
    return this->reducersAvailable[k];
}

void ShufflerExecutionMessage::setReducersAvailableArraySize(size_t newSize)
{
    int *reducersAvailable2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = reducersAvailable_arraysize < newSize ? reducersAvailable_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        reducersAvailable2[i] = this->reducersAvailable[i];
    for (size_t i = minSize; i < newSize; i++)
        reducersAvailable2[i] = 0;
    delete [] this->reducersAvailable;
    this->reducersAvailable = reducersAvailable2;
    reducersAvailable_arraysize = newSize;
}

void ShufflerExecutionMessage::setReducersAvailable(size_t k, int reducersAvailable)
{
    if (k >= reducersAvailable_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)reducersAvailable_arraysize, (unsigned long)k);
    this->reducersAvailable[k] = reducersAvailable;
}

void ShufflerExecutionMessage::insertReducersAvailable(size_t k, int reducersAvailable)
{
    if (k > reducersAvailable_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)reducersAvailable_arraysize, (unsigned long)k);
    size_t newSize = reducersAvailable_arraysize + 1;
    int *reducersAvailable2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        reducersAvailable2[i] = this->reducersAvailable[i];
    reducersAvailable2[k] = reducersAvailable;
    for (i = k + 1; i < newSize; i++)
        reducersAvailable2[i] = this->reducersAvailable[i-1];
    delete [] this->reducersAvailable;
    this->reducersAvailable = reducersAvailable2;
    reducersAvailable_arraysize = newSize;
}

void ShufflerExecutionMessage::appendReducersAvailable(int reducersAvailable)
{
    insertReducersAvailable(reducersAvailable_arraysize, reducersAvailable);
}

void ShufflerExecutionMessage::eraseReducersAvailable(size_t k)
{
    if (k >= reducersAvailable_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)reducersAvailable_arraysize, (unsigned long)k);
    size_t newSize = reducersAvailable_arraysize - 1;
    int *reducersAvailable2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        reducersAvailable2[i] = this->reducersAvailable[i];
    for (i = k; i < newSize; i++)
        reducersAvailable2[i] = this->reducersAvailable[i+1];
    delete [] this->reducersAvailable;
    this->reducersAvailable = reducersAvailable2;
    reducersAvailable_arraysize = newSize;
}

size_t ShufflerExecutionMessage::getPartitionsElaboratedArraySize() const
{
    return partitionsElaborated_arraysize;
}

int ShufflerExecutionMessage::getPartitionsElaborated(size_t k) const
{
    if (k >= partitionsElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionsElaborated_arraysize, (unsigned long)k);
    return this->partitionsElaborated[k];
}

void ShufflerExecutionMessage::setPartitionsElaboratedArraySize(size_t newSize)
{
    int *partitionsElaborated2 = (newSize==0) ? nullptr : new int[newSize];
    size_t minSize = partitionsElaborated_arraysize < newSize ? partitionsElaborated_arraysize : newSize;
    for (size_t i = 0; i < minSize; i++)
        partitionsElaborated2[i] = this->partitionsElaborated[i];
    for (size_t i = minSize; i < newSize; i++)
        partitionsElaborated2[i] = 0;
    delete [] this->partitionsElaborated;
    this->partitionsElaborated = partitionsElaborated2;
    partitionsElaborated_arraysize = newSize;
}

void ShufflerExecutionMessage::setPartitionsElaborated(size_t k, int partitionsElaborated)
{
    if (k >= partitionsElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionsElaborated_arraysize, (unsigned long)k);
    this->partitionsElaborated[k] = partitionsElaborated;
}

void ShufflerExecutionMessage::insertPartitionsElaborated(size_t k, int partitionsElaborated)
{
    if (k > partitionsElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionsElaborated_arraysize, (unsigned long)k);
    size_t newSize = partitionsElaborated_arraysize + 1;
    int *partitionsElaborated2 = new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        partitionsElaborated2[i] = this->partitionsElaborated[i];
    partitionsElaborated2[k] = partitionsElaborated;
    for (i = k + 1; i < newSize; i++)
        partitionsElaborated2[i] = this->partitionsElaborated[i-1];
    delete [] this->partitionsElaborated;
    this->partitionsElaborated = partitionsElaborated2;
    partitionsElaborated_arraysize = newSize;
}

void ShufflerExecutionMessage::appendPartitionsElaborated(int partitionsElaborated)
{
    insertPartitionsElaborated(partitionsElaborated_arraysize, partitionsElaborated);
}

void ShufflerExecutionMessage::erasePartitionsElaborated(size_t k)
{
    if (k >= partitionsElaborated_arraysize) throw omnetpp::cRuntimeError("Array of size %lu indexed by %lu", (unsigned long)partitionsElaborated_arraysize, (unsigned long)k);
    size_t newSize = partitionsElaborated_arraysize - 1;
    int *partitionsElaborated2 = (newSize == 0) ? nullptr : new int[newSize];
    size_t i;
    for (i = 0; i < k; i++)
        partitionsElaborated2[i] = this->partitionsElaborated[i];
    for (i = k; i < newSize; i++)
        partitionsElaborated2[i] = this->partitionsElaborated[i+1];
    delete [] this->partitionsElaborated;
    this->partitionsElaborated = partitionsElaborated2;
    partitionsElaborated_arraysize = newSize;
}

class ShufflerExecutionMessageDescriptor : public omnetpp::cClassDescriptor
{
  private:
    mutable const char **propertyNames;
    enum FieldConstants {
        FIELD_reducersAvailable,
        FIELD_partitionsElaborated,
    };
  public:
    ShufflerExecutionMessageDescriptor();
    virtual ~ShufflerExecutionMessageDescriptor();

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

Register_ClassDescriptor(ShufflerExecutionMessageDescriptor)

ShufflerExecutionMessageDescriptor::ShufflerExecutionMessageDescriptor() : omnetpp::cClassDescriptor(omnetpp::opp_typename(typeid(ShufflerExecutionMessage)), "omnetpp::cMessage")
{
    propertyNames = nullptr;
}

ShufflerExecutionMessageDescriptor::~ShufflerExecutionMessageDescriptor()
{
    delete[] propertyNames;
}

bool ShufflerExecutionMessageDescriptor::doesSupport(omnetpp::cObject *obj) const
{
    return dynamic_cast<ShufflerExecutionMessage *>(obj)!=nullptr;
}

const char **ShufflerExecutionMessageDescriptor::getPropertyNames() const
{
    if (!propertyNames) {
        static const char *names[] = {  nullptr };
        omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
        const char **baseNames = base ? base->getPropertyNames() : nullptr;
        propertyNames = mergeLists(baseNames, names);
    }
    return propertyNames;
}

const char *ShufflerExecutionMessageDescriptor::getProperty(const char *propertyName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? base->getProperty(propertyName) : nullptr;
}

int ShufflerExecutionMessageDescriptor::getFieldCount() const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    return base ? 2+base->getFieldCount() : 2;
}

unsigned int ShufflerExecutionMessageDescriptor::getFieldTypeFlags(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeFlags(field);
        field -= base->getFieldCount();
    }
    static unsigned int fieldTypeFlags[] = {
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_reducersAvailable
        FD_ISARRAY | FD_ISEDITABLE | FD_ISRESIZABLE,    // FIELD_partitionsElaborated
    };
    return (field >= 0 && field < 2) ? fieldTypeFlags[field] : 0;
}

const char *ShufflerExecutionMessageDescriptor::getFieldName(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldName(field);
        field -= base->getFieldCount();
    }
    static const char *fieldNames[] = {
        "reducersAvailable",
        "partitionsElaborated",
    };
    return (field >= 0 && field < 2) ? fieldNames[field] : nullptr;
}

int ShufflerExecutionMessageDescriptor::findField(const char *fieldName) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    int baseIndex = base ? base->getFieldCount() : 0;
    if (strcmp(fieldName, "reducersAvailable") == 0) return baseIndex + 0;
    if (strcmp(fieldName, "partitionsElaborated") == 0) return baseIndex + 1;
    return base ? base->findField(fieldName) : -1;
}

const char *ShufflerExecutionMessageDescriptor::getFieldTypeString(int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldTypeString(field);
        field -= base->getFieldCount();
    }
    static const char *fieldTypeStrings[] = {
        "int",    // FIELD_reducersAvailable
        "int",    // FIELD_partitionsElaborated
    };
    return (field >= 0 && field < 2) ? fieldTypeStrings[field] : nullptr;
}

const char **ShufflerExecutionMessageDescriptor::getFieldPropertyNames(int field) const
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

const char *ShufflerExecutionMessageDescriptor::getFieldProperty(int field, const char *propertyName) const
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

int ShufflerExecutionMessageDescriptor::getFieldArraySize(omnetpp::any_ptr object, int field) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldArraySize(object, field);
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: return pp->getReducersAvailableArraySize();
        case FIELD_partitionsElaborated: return pp->getPartitionsElaboratedArraySize();
        default: return 0;
    }
}

void ShufflerExecutionMessageDescriptor::setFieldArraySize(omnetpp::any_ptr object, int field, int size) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldArraySize(object, field, size);
            return;
        }
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: pp->setReducersAvailableArraySize(size); break;
        case FIELD_partitionsElaborated: pp->setPartitionsElaboratedArraySize(size); break;
        default: throw omnetpp::cRuntimeError("Cannot set array size of field %d of class 'ShufflerExecutionMessage'", field);
    }
}

const char *ShufflerExecutionMessageDescriptor::getFieldDynamicTypeString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldDynamicTypeString(object,field,i);
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        default: return nullptr;
    }
}

std::string ShufflerExecutionMessageDescriptor::getFieldValueAsString(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValueAsString(object,field,i);
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: return long2string(pp->getReducersAvailable(i));
        case FIELD_partitionsElaborated: return long2string(pp->getPartitionsElaborated(i));
        default: return "";
    }
}

void ShufflerExecutionMessageDescriptor::setFieldValueAsString(omnetpp::any_ptr object, int field, int i, const char *value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValueAsString(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: pp->setReducersAvailable(i,string2long(value)); break;
        case FIELD_partitionsElaborated: pp->setPartitionsElaborated(i,string2long(value)); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShufflerExecutionMessage'", field);
    }
}

omnetpp::cValue ShufflerExecutionMessageDescriptor::getFieldValue(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldValue(object,field,i);
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: return pp->getReducersAvailable(i);
        case FIELD_partitionsElaborated: return pp->getPartitionsElaborated(i);
        default: throw omnetpp::cRuntimeError("Cannot return field %d of class 'ShufflerExecutionMessage' as cValue -- field index out of range?", field);
    }
}

void ShufflerExecutionMessageDescriptor::setFieldValue(omnetpp::any_ptr object, int field, int i, const omnetpp::cValue& value) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldValue(object, field, i, value);
            return;
        }
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        case FIELD_reducersAvailable: pp->setReducersAvailable(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        case FIELD_partitionsElaborated: pp->setPartitionsElaborated(i,omnetpp::checked_int_cast<int>(value.intValue())); break;
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShufflerExecutionMessage'", field);
    }
}

const char *ShufflerExecutionMessageDescriptor::getFieldStructName(int field) const
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

omnetpp::any_ptr ShufflerExecutionMessageDescriptor::getFieldStructValuePointer(omnetpp::any_ptr object, int field, int i) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount())
            return base->getFieldStructValuePointer(object, field, i);
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        default: return omnetpp::any_ptr(nullptr);
    }
}

void ShufflerExecutionMessageDescriptor::setFieldStructValuePointer(omnetpp::any_ptr object, int field, int i, omnetpp::any_ptr ptr) const
{
    omnetpp::cClassDescriptor *base = getBaseClassDescriptor();
    if (base) {
        if (field < base->getFieldCount()){
            base->setFieldStructValuePointer(object, field, i, ptr);
            return;
        }
        field -= base->getFieldCount();
    }
    ShufflerExecutionMessage *pp = omnetpp::fromAnyPtr<ShufflerExecutionMessage>(object); (void)pp;
    switch (field) {
        default: throw omnetpp::cRuntimeError("Cannot set field %d of class 'ShufflerExecutionMessage'", field);
    }
}

namespace omnetpp {

}  // namespace omnetpp

