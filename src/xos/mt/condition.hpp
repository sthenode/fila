///////////////////////////////////////////////////////////////////////
/// Copyright (c) 1988-2019 $organization$
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
///   File: condition.hpp
///
/// Author: $author$
///   Date: 8/19/2019
///////////////////////////////////////////////////////////////////////
#ifndef _XOS_MT_CONDITION_HPP
#define _XOS_MT_CONDITION_HPP

#include "xos/base/logged.hpp"
#include "xos/base/waited.hpp"
#include "xos/base/signaled.hpp"
#include "xos/base/created.hpp"
#include "xos/mt/mutex.hpp"

#define XOS_MT_CONDITION_CREATED() \
    IS_ERR_LOGGED_DEBUG("this->created()..."); \
    if (!(this->created())) { \
        IS_ERR_LOGGED_ERROR("...failed on this->created() throw create_exception(create_failed)..."); \
        throw create_exception(create_failed); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->created()"); \
    }
    
#define XOS_MT_CONDITION_DESTROYED() \
    IS_ERR_LOGGED_DEBUG("this->destroyed()..."); \
    if (!(this->destroyed())) { \
        IS_ERR_LOGGED_ERROR("...failed on this->destroyed() throw create_exception(destroy_failed)..."); \
        throw create_exception(destroy_failed); \
    } else { \
        IS_ERR_LOGGED_DEBUG("...this->destroyed()"); \
    }

namespace xos {
namespace mt {

///////////////////////////////////////////////////////////////////////
///  Class: conditiont
///////////////////////////////////////////////////////////////////////
template 
<class TImplement = loggedt<signaled>,
 class TImplements = creatort<TImplement> >

class _EXPORT_CLASS conditiont: virtual public TImplements {
public:
    typedef TImplements implements;
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS conditiont
typedef conditiont<> condition;

namespace extended {
///////////////////////////////////////////////////////////////////////
///  Class: conditiont
///////////////////////////////////////////////////////////////////////
template 
<typename TAttachedTo = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TImplement = mt::condition,  class TExtend = ::xos::extended::loggedt<TImplement, extend>,
 class TAttacher = attachert<TAttachedTo, TUnattached, VUnattached, TImplement>,
 class TAttached = attachedt<TAttachedTo, TUnattached, VUnattached, TAttacher, TExtend>,
 class TCreated = createdt<TAttachedTo, TUnattached, VUnattached, TAttacher, TAttached>,
 class TImplements = TAttacher, class TExtends = TCreated>

class _EXPORT_CLASS conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef TAttachedTo attached_t;
    typedef TUnattached unattached_t;
    enum { unattached = VUnattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    conditiont(attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_err_logged);
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_logged);
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(attached_t detached): extends(detached) {
    }
    conditiont(bool is_logged, bool is_err_logged) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_err_logged);
    }
    conditiont(bool is_logged) {
        this->set_is_logged(is_logged);
        this->set_is_err_logged(is_logged);
    }
    conditiont(const conditiont &copy): extends(copy) {
        this->set_is_logged(copy.is_logged());
        this->set_is_err_logged(copy.is_err_logged());
    }
    conditiont() {
    }
    virtual ~conditiont() {
        XOS_MT_CONDITION_DESTROYED();
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    using extends::create;
    virtual bool create(bool initially_signaled) {
        attached_t detached = ((attached_t)unattached);
        if (((attached_t)unattached) != (detached = create_attached(initially_signaled))) {
            this->set_is_created();
            return true;
        }
        return false;
    }
    using extends::create_attached;
    virtual attached_t create_attached(bool initially_signaled) {
        attached_t detached = ((attached_t)unattached);
        if (!(initially_signaled)) {
            detached = create_attached();
        } else {
            if ((detached = create_detached(initially_signaled))) {
                this->attach(detached);
            }
        }
        return detached;
    }
    using extends::create_detached;
    virtual attached_t create_detached(bool initially_signaled) const {
        attached_t detached = ((attached_t)unattached);
        if (!(initially_signaled)) {
            detached = create_detached();
        }
        return detached;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS conditiont
typedef conditiont<> condition;
} /// namespace extended

namespace derived {
///////////////////////////////////////////////////////////////////////
///  Class: conditiont
///////////////////////////////////////////////////////////////////////
template 
<typename TAttached = pointer_t, typename TUnattached = int, TUnattached VUnattached = 0,
 class TExtends = extended::conditiont<TAttached, TUnattached, VUnattached>, 
 class TImplements = typename TExtends::implements>
class _EXPORT_CLASS conditiont: virtual public TImplements, public TExtends {
public:
    typedef TImplements implements;
    typedef TExtends extends;

    typedef derived::mutex mutex_t;
    typedef typename extends::attached_t attached_t;
    typedef typename extends::unattached_t unattached_t;
    enum { unattached = extends::unattached };

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(mutex_t& mutex, bool& signaled, attached_t detached): extends(detached) {
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(mutex_t& mutex, bool& signaled, bool is_logged): extends(is_logged) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(mutex_t& mutex, bool& signaled) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(attached_t detached, bool is_created, bool is_logged, bool is_err_logged): extends(detached, is_created, is_logged, is_err_logged) {
    }
    conditiont(attached_t detached, bool is_created, bool is_logged): extends(detached, is_created, is_logged) {
    }
    conditiont(attached_t detached, bool is_created): extends(detached, is_created) {
    }
    conditiont(attached_t detached): extends(detached) {
    }
    conditiont(bool is_logged, bool is_err_logged): extends(is_logged, is_err_logged) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(bool is_logged): extends(is_logged) {
        XOS_MT_CONDITION_CREATED();
    }
    conditiont(const conditiont &copy): extends(copy) {
    }
    conditiont() {
        XOS_MT_CONDITION_CREATED();
    }
    virtual ~conditiont() {
        XOS_MT_CONDITION_DESTROYED();
    }
    
    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
    virtual attached_t create_detached(bool initially_signaled) const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual attached_t create_detached() const {
        attached_t detached = (attached_t)(unattached);
        return detached;
    }
    virtual bool destroy_detached(attached_t detached) {
        if ((attached_t)(unattached) != (detached)) {
        }
        return false;
    }

    ///////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////
}; /// class _EXPORT_CLASS conditiont
typedef conditiont<> condition;
} /// namespace derived

} /// namespace mt
} /// namespace xos

#endif /// _XOS_MT_CONDITION_HPP 
