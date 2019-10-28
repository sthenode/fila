///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2018 $organization$
///
/// This software is provided by the author and contributors ``as is'' 
/// and any express or implied warranties, including, but not limited to, 
/// the implied warranties of merchantability and fitness for a particular 
/// purpose are disclaimed. In no event shall the author or contributors 
/// be liable for any direct, indirect, incidental, special, exemplary, 
/// or consequential damages (including, but not limited to, procurement 
/// of substitute goods or services; loss of use, data, or profits; or 
/// business interruption) however caused and on any theory of liability, 
/// whether in contract, strict liability, or tort (including negligence 
/// or otherwise) arising in any way out of the use of this software, 
/// even if advised of the possibility of such damage.
///
///   File: thread.hpp
///
/// Author: $author$
///   Date: 4/11/2018
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_THREAD_HPP
#define _XOS_MT_THREAD_HPP

#include "xos/base/ran.hpp"
#include "xos/base/suspended.hpp"
#include "xos/base/joined.hpp"
#include "xos/base/logged.hpp"
#include "xos/base/created.hpp"
#include "xos/io/logger.hpp"

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: threadt_joinedt
///////////////////////////////////////////////////////////////////////
template <class TRan = ran, class TSuspended = suspended, class TJoined = joined>
class _EXPORT_CLASS threadt_joinedt
: virtual public TRan, virtual public TSuspended, virtual public TJoined {
public:
    typedef TRan ran_t;
};
typedef threadt_joinedt<> threadt_joined;

///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template <class TJoined = threadt_joined, class TLogged = loggedt<TJoined>, class TImplements = creatort<TJoined> >
class _EXPORT_CLASS threadt: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef typename implements::ran_t ran_t;
};
typedef threadt<> thread;

namespace implemented {
///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplements = attachert<TAttachedTo, TUnattached, VUnattached, mt::thread> >
class _EXPORT_CLASS threadt: virtual public TImplements {
public:
    typedef TImplements implements;
    typedef threadt derives;

    typedef typename implements::ran_t ran_t;
    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    using implements::created;
    virtual bool created(bool initially_suspended) {
        if ((initially_suspended)) {
            if ((!this->is_created())) {
                if ((this->create(initially_suspended))) {
                    return true;
                }
            }
        } else {
            if ((this->created())) {
                return true;
            }
        }
        return false;
    }
    using implements::create;
    virtual bool create(bool initially_suspended) {
        return false;
    }
    virtual attached_t detach() {
        attached_t detached = implements::detach();
        this->set_is_forked(false);
        return detached;
    }
}; /// class _EXPORT_CLASS threadt
typedef threadt<> thread;
} /// namespace implemented

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template 
<typename TAttachedTo = pointer_t, 
 typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = implemented::threadt<TAttachedTo, TUnattached, VUnattached>,
 class TLogged = xos::extended::loggedt<TImplement, extend>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TImplement, TLogged>,
 class TExtends = createdt<TAttachedTo, TUnattached, VUnattached, TImplement, TAttached>,
 class TImplements = typename TExtends::implements>
class _EXPORT_CLASS threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;

    typedef typename implements::ran_t ran_t;
    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    threadt(attached_t attached, bool forked, bool is_logged, bool is_err_logged)
    : extends(attached, forked), ran_(*this), forked_(forked) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_err_logged;
    }
    threadt(attached_t attached, bool forked, bool is_logged)
    : extends(attached, forked), ran_(*this), forked_(forked) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_logged;
    }
    threadt(attached_t attached, bool forked)
    : extends(attached, forked), ran_(*this), forked_(forked) {
    }
    threadt(attached_t attached)
    : extends(attached), ran_(*this), forked_(false) {
    }
    threadt(ran_t& ran, bool is_logged, bool is_err_logged): ran_(ran), forked_(false) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_err_logged;
    }
    threadt(ran_t& ran, bool is_logged): ran_(ran), forked_(false) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_logged;
    }
    threadt(ran_t& ran): ran_(ran), forked_(false) {
    }
    threadt(bool is_logged, bool is_err_logged): ran_(*this), forked_(false) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_err_logged;
    }
    threadt(bool is_logged): ran_(*this), forked_(false) {
        this->is_logged_ = is_logged;
        this->is_err_logged_ = is_logged;
    }
    threadt(const threadt& copy)
    : extends(copy), ran_(*this), forked_(false) {
    }
    threadt(): ran_(*this), forked_(false) {
    }
    virtual ~threadt() {
        if (!(this->destroyed())) {
            throw create_exception(destroy_failed);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool create(bool initially_suspended) {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = create_attached(initially_suspended))) {
            this->set_is_created();
            this->set_is_forked();
           return true;
        }
        return false;
    }
    virtual bool create() {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = create_attached())) {
            this->set_is_created();
            this->set_is_forked();
            return true;
        }
        return false;
    }
    virtual bool destroy() {
        if ((this->joined())) {
            this->detach();
            return true;
        }
        return false;
    }
    virtual attached_t create_attached(bool initially_suspended) {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if ((attached_t)(unattached) != (detached = create_detached(initially_suspended))) {
                this->attach(detached);
                return detached;
            }
        }
        return detached;
    }
    virtual attached_t create_attached() {
        attached_t detached = (attached_t)(unattached);
        if ((this->destroyed())) {
            if ((attached_t)(unattached) != (detached = create_detached())) {
                this->attach(detached);
                return detached;
            }
        }
        return detached;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool join() {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = this->attached_to())) {
            return join_detached(forked_, detached);
        }
        return false;
    }
    virtual join_status untimed_join() {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = this->attached_to())) {
            return untimed_join_detached(forked_, detached);
        }
        return join_failed;
    }
    virtual join_status timed_join(mseconds_t milliseconds) {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = this->attached_to())) {
            return timed_join_detached(forked_, detached, milliseconds);
        }
        return join_failed;
    }
    virtual join_status try_join() {
        attached_t detached = (attached_t)(unattached);
        if ((attached_t)(unattached) != (detached = this->attached_to())) {
            return try_join_detached(forked_, detached);
        }
        return join_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool join_detached(bool& forked, attached_t detached) const {
        if (join_success == (untimed_join_detached(forked, detached))) {
            return true;
        }
        return false;
    }
    virtual join_status untimed_join_detached(bool& forked, attached_t detached) const {
        return join_failed;
    }
    virtual join_status timed_join_detached
    (bool& forked, attached_t detached, mseconds_t milliseconds) const {
        return join_failed;
    }
    virtual join_status try_join_detached(bool& forked, attached_t detached) const {
        return join_failed;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(bool initially_suspended) const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual attached_t create_detached() const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) const {
        if ((attached_t)(unattached) != (detached)) {
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual bool set_is_forked(bool to = true) {
        forked_ = to;
        return is_forked();
    }
    virtual bool is_forked() const {
        return forked_;
    }

protected:
    ran_t& ran_;
    bool forked_;
};
typedef threadt<> thread;
} /// namespace extended

namespace derived {
///////////////////////////////////////////////////////////////////////
///  Class: threadt
///////////////////////////////////////////////////////////////////////
template 
<class TExtends = extended::threadt<pointer_t>, class TImplements = typename TExtends::implements>
class _EXPORT_CLASS threadt: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;
    typedef threadt derives;
    
    typedef typename implements::ran_t ran_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    threadt(attached_t detached, bool is_created, bool is_logged, bool is_err_logged)
    : extends(detached, is_created, is_logged, is_err_logged) {
    }
    threadt(attached_t detached, bool is_created, bool is_logged)
    : extends(detached, is_created, is_logged) {
    }
    threadt(attached_t detached, bool is_created)
    : extends(detached, is_created) {
    }
    threadt(attached_t detached)
    : extends(detached) {
    }
    threadt(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(bool is_logged): extends(is_logged) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(ran_t& ran): extends(ran) {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    threadt(const threadt &copy): extends(copy) {
    }
    threadt() {
        if (!(this->created())) {
            throw create_exception(create_failed);
        }
    }
    virtual ~threadt() {
        if (!(this->destroyed())) {
            throw create_exception(destroy_failed);
        }
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS threadt
typedef threadt<> thread;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_THREAD_HPP 
