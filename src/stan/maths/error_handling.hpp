#ifndef __STAN__MATHS__ERROR_HANDLING_HPP__
#define __STAN__MATHS__ERROR_HANDLING_HPP__

#include <limits>

#include <boost/math/policies/policy.hpp>
#include <boost/math/policies/error_handling.hpp>
#include <boost/math/distributions/detail/common_error_handling.hpp>

#include <stan/maths/matrix.hpp>
#include <stan/meta/traits.hpp>
#include <stan/prob/transform.hpp>

namespace stan { 

  namespace maths {

    /**
     * Default error-handling policy from Boost.
     */
    typedef boost::math::policies::policy<> default_policy;

    /**
     * Checks if the variable y is nan.
     */
    template <typename T_y, typename T_result, class Policy>
    inline bool check_not_nan(const char* function,
                              const T_y& y,
                              const char* name,
                              T_result* result,
                              const Policy& /* pol */) {
      using boost::math::policies::raise_domain_error;
      if (boost::math::isnan(y)) {
        std::string msg_str(name);
        msg_str += " is %1%, but must not be nan!";
        const char* msg = msg_str.c_str();
        *result = raise_domain_error<T_y>(function,msg,y,Policy());
        return false;
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy>
    inline bool check_not_nan(const char* function,
                              const std::vector<T_y>& y,
                              const char* name,
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      for (int i = 0; i < y.size(); i++) {
        if (boost::math::isnan(y[i])) {
          std::ostringstream msg_o;
          msg_o << name << "[" << i << "] is %1%, but must not be nan!";
          const char* msg = msg_o.str().c_str();
          *result = raise_domain_error<T_y>(function,msg,y[i],Policy());
          return false;
        }
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy>
    inline bool check_not_nan(const char* function,
                              const typename 
                              stan::maths::EigenType<T_y>::vector& y,
                              const char* name,
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      for (int i = 0; i < y.rows(); i++) {
        if (boost::math::isnan(y[i])) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must not be nan!";
          *result = raise_domain_error<T_y>(function,
                                            message.str().c_str(),
                                            y[i], Policy());
          return false;
        }
      }
      return true;
    }


    /**
     * Checks if the variable y is finite.
     */
    template <typename T_y, typename T_result, class Policy>
    inline bool check_finite(const char* function,
                             const T_y& y,
                             const char* name,
                             T_result* result,
                             const Policy& pol) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(y)) {
        std::string message(name);
        message += " is %1%, but must be finite!";
        *result = raise_domain_error<T_y>(function,
                                          message.c_str(), 
                                          y, Policy());
        return false;
      }
      return true;
    }
    
    template <typename T_y, typename T_result, class Policy>
    inline bool check_finite(const char* function,
                             const std::vector<T_y>& y,
                             const char* name,
                             T_result* result,
                             const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      for (int i = 0; i < y.size(); i++) {
        if (!boost::math::isfinite(y[i])) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must be finite!";
          *result = raise_domain_error<T_y>(function,
                                            message.str().c_str(),
                                            y[i], Policy());
          return false;
        }
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy>
    inline bool check_finite(const char* function,
                             const typename 
                             stan::maths::EigenType<T_y>::vector& y,
                             const char* name,
                             T_result* result,
                             const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      for (int i = 0; i < y.rows(); i++) {
        if (!boost::math::isfinite(y[i])) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must be finite!";
          *result = raise_domain_error<T_y>(function,
                                            message.str().c_str(),
                                            y[i], Policy());
          return false;
        }
      }
      return true;
    }

    template <typename T_x, typename T_low, typename T_result, class Policy>
    inline bool check_greater(const char* function,
                              const T_x& x,
                              const T_low& low,
                              const char* name,  
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      using boost::math::tools::promote_args;
      if (!boost::math::isfinite(x) || !(x > low)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be finite and greater than "
            << low;
        *result = raise_domain_error<typename promote_args<T_x>::type>
                      (function, msg.str().c_str(), x, Policy());
        return false;
      }
      return true;
    }


    template <typename T_x, typename T_low, typename T_high, typename T_result,
              class Policy>
    inline bool check_bounded(const char* function,
                              const T_x& x,
                              const T_low& low,
                              const T_high& high,
                              const char* name,  
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(x) || !(low <= x && x <= high)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be finite and between "
            << low
            << " and "
            << high;
        *result = raise_domain_error<typename boost::math::tools::promote_args<T_x>::type >
          (function,
           msg.str().c_str(),
           x, Policy());
        return false;
      }
      return true;
    }
    template <typename T_low, typename T_high, typename T_result, class Policy>
    inline bool check_bounded(const char* function,
                              const unsigned int x,
                              const T_low& low,
                              const T_high& high,
                              const char* name,  
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!(low <= x && x <= high)) {
        std::ostringstream msg;
        msg << name 
            << " is %1%, but must be finite and between "
            << low
            << " and "
            << high;
        *result = raise_domain_error<double>(function,
                                             msg.str().c_str(),
                                             x, Policy());
        return false;
      }
      return true;
    }

    template <typename T_scale, typename T_result, class Policy>
    inline bool check_scale(const char* function,
                            const T_scale& scale,
                            T_result* result,
                            const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      // Assume scale == 0 is NOT valid for any distribution.
      if (!(scale > 0) || !boost::math::isfinite(scale)) { 
        *result = raise_domain_error<T_scale>(function,
                                              "Scale parameter is %1%, but must be > 0 !", 
                                              scale, Policy());
        return false;
      }
      return true;
    }

    template <typename T_inv_scale, typename T_result, class Policy>
    inline bool check_inv_scale(const char* function,
                                const T_inv_scale& invScale,
                                T_result* result,
                                const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!(invScale > 0)
          || !boost::math::isfinite(invScale)) { // Assume scale == 0 is NOT valid for any distribution.
        *result = raise_domain_error<T_inv_scale>(function,
                                                  "Inverse scale parameter is %1%, but must be > 0 !", 
                                                  invScale, Policy());
        return false;
      }
      return true;
    }


    template <typename T_x, typename T_result, class Policy>
    inline bool check_nonnegative(const char* function,
                                  const T_x& x,
                                  const char* name,
                                  T_result* result,
                                  const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(x) || !(x >= 0)) {
        std::string message(name);
        message += " is %1%, but must be finite and >= 0!";
        *result = raise_domain_error<typename boost::math::tools::promote_args<T_x>::type >(function,
                                                                                            message.c_str(), 
                                                                                            x, Policy());
        return false;
      }
      return true;
    }

    template <typename T_result, class Policy>
    inline bool check_nonnegative(const char* function,
                                  const unsigned int& x,
                                  const char* name,
                                  T_result* result,
                                  const Policy& /*pol*/) {
      return true;
    }


    template <typename T_x, typename T_result, class Policy>
    inline bool check_positive(const char* function,
                               const T_x& x,
                               const char* name,
                               T_result* result,
                               const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(x) || !(x > 0)) {
        std::string message(name);
        message += " is %1%, but must be finite and > 0!";
        *result = raise_domain_error<T_x>(function,
                                          message.c_str(), 
                                          x, Policy());
        
        return false;
      }
      return true;
    }

    template <typename T_y, typename T_result, class Policy>
    inline bool check_positive(const char* function,
                               const std::vector<T_y>& y,
                               const char* name,
                               T_result* result,
                               const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      for (int i = 0; i < y.size(); i++) {
        if (!boost::math::isfinite(y[i]) || !(y[i] > 0)) {
          std::ostringstream message;
          message << name << "[" << i << "] is %1%, but must be finite and > 0!";
          *result = raise_domain_error<T_y>(function,
                                            message.str().c_str(),
                                            y[i], Policy());
          return false;
        }
      }
      return true;
    }


    template <typename T_location, typename T_result, class Policy>
    inline bool check_location(const char* function,
                               const T_location& location,
                               T_result* result,
                               const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(location)) {
        *result = raise_domain_error<T_location>(function,
                                                 "Location parameter is %1%, but must be finite!", 
                                                 location, Policy());
        return false;
      }
      return true;
    }

    template <typename T_bound, typename T_result, class Policy>
    inline bool check_lower_bound(const char* function,
                                  const T_bound& lb,
                                  T_result* result,
                                  const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(lb)) {
        *result = raise_domain_error<T_bound>(function,
                                              "Lower bound is %1%, but must be finite!", 
                                              lb, Policy());
        return false;
      }
      return true;
    }


    template <typename T_bound, typename T_result, class Policy>
    inline bool check_upper_bound(const char* function,
                                  const T_bound& ub,
                                  T_result* result,
                                  const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!boost::math::isfinite(ub)) {
        *result = raise_domain_error<T_bound>(function,
                                              "Upper bound is %1%, but must be finite!", 
                                              ub, Policy());
        return false;
      }
      return true;
    }

    template <typename T_lb, typename T_ub, typename T_result, class Policy>
    inline bool check_bounds(const char* function,
                             const T_lb& lower,
                             const T_ub& upper,
                             T_result* result,
                             const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!check_lower_bound(function, lower, result, Policy()))
        return false;
      if (!check_upper_bound(function, upper, result, Policy()))
        return false;
      if (lower >= upper) {
        *result = raise_domain_error<T_lb>(function,
                                           "lower parameter is %1%, but must be less than upper!", 
                                           lower, Policy());
        return false;
      }
      return true;
    }

    template <typename T_result, class Policy>
    inline bool check_size_match(const char* function,
                                 unsigned int i,
                                 unsigned int j,
                                 T_result* result,
                                 const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (i != j) {
        std::ostringstream msg;
        msg << "i and j must be same.  Found i=%1%, j=" << j;
        *result = raise_domain_error<double>(function,
                                             msg.str().c_str(),
                                             i,
                                             Policy());
        return false;
      }
      return true;
    }
    
    template <typename T_covar, typename T_result, class Policy>
    inline bool check_cov_matrix(const char* function,
                                 const typename stan::maths::EigenType<T_covar>::matrix& Sigma,
                                 T_result* result,
                                 const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!cov_matrix_validate(Sigma)) {
        std::ostringstream stream;
        stream << "Sigma is not a valid covariance matrix."
               << " Sigma must be symmetric and positive semi-definite."
               << " Sigma:" << std::endl
               << Sigma << std::endl
               << "Sigma(0,0): %1%";
        *result = raise_domain_error<T_covar>(function,
                                              stream.str().c_str(), 
                                              Sigma(0,0),
                                              Policy());
        return false;
      }
      return true;
    }

    template <typename T_prob, typename T_result, class Policy>
    inline bool check_simplex(const char* function,
                              const typename stan::maths::EigenType<T_prob>::vector& theta,
                              const char* name,
                              T_result* result,
                              const Policy& /*pol*/) {
      using boost::math::policies::raise_domain_error;
      if (!simplex_validate(theta)) {
        std::ostringstream stream;
        stream << name
               << "is not a valid simplex."
               << " The first element of the simplex is: %1%.";
        *result = raise_domain_error<T_prob>(function,
                                             stream.str().c_str(), 
                                             theta(0),
                                             Policy());
        return false;
      }
      return true;
    }
    
  }
}

#endif


