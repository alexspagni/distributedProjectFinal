//
// Generated file, do not edit! Created by opp_msgtool 6.0 from backOnline.msg.
//

#ifndef __BACKONLINE_M_H
#define __BACKONLINE_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// opp_msgtool version check
#define MSGC_VERSION 0x0600
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of opp_msgtool: 'make clean' should help.
#endif

class backOnline;
/**
 * Class generated from <tt>backOnline.msg:16</tt> by opp_msgtool.
 * <pre>
 * //
 * // This program is free software: you can redistribute it and/or modify
 * // it under the terms of the GNU Lesser General Public License as published by
 * // the Free Software Foundation, either version 3 of the License, or
 * // (at your option) any later version.
 * // 
 * // This program is distributed in the hope that it will be useful,
 * // but WITHOUT ANY WARRANTY; without even the implied warranty of
 * // MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * // GNU Lesser General Public License for more details.
 * // 
 * // You should have received a copy of the GNU Lesser General Public License
 * // along with this program.  If not, see http://www.gnu.org/licenses/.
 * //
 * message backOnline
 * {
 *     bool performed;
 *     int nodeNumber;
 *     string partition;
 * }
 * </pre>
 */
class backOnline : public ::omnetpp::cMessage
{
  protected:
    bool performed = false;
    int nodeNumber = 0;
    omnetpp::opp_string partition;

  private:
    void copy(const backOnline& other);

  protected:
    bool operator==(const backOnline&) = delete;

  public:
    backOnline(const char *name=nullptr, short kind=0);
    backOnline(const backOnline& other);
    virtual ~backOnline();
    backOnline& operator=(const backOnline& other);
    virtual backOnline *dup() const override {return new backOnline(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    virtual bool getPerformed() const;
    virtual void setPerformed(bool performed);

    virtual int getNodeNumber() const;
    virtual void setNodeNumber(int nodeNumber);

    virtual const char * getPartition() const;
    virtual void setPartition(const char * partition);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const backOnline& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, backOnline& obj) {obj.parsimUnpack(b);}


namespace omnetpp {

template<> inline backOnline *fromAnyPtr(any_ptr ptr) { return check_and_cast<backOnline*>(ptr.get<cObject>()); }

}  // namespace omnetpp

#endif // ifndef __BACKONLINE_M_H

