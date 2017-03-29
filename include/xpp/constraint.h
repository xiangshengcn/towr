/**
 @file    constraint.h
 @author  Alexander W. Winkler (winklera@ethz.ch)
 @date    May 30, 2016
 @brief   Abstract class representing a constraint for the NLP problem.
 */

#ifndef XPP_XPP_OPT_INCLUDE_XPP_OPT_CONSTRAINT_H_
#define XPP_XPP_OPT_INCLUDE_XPP_OPT_CONSTRAINT_H_

#include "optimization_variables.h"
#include "parametrization.h"

#include <Eigen/Sparse> // for jacobians
#include <memory>

namespace xpp {
namespace opt {

/** Common interface providing constraint values and bounds.
  */
class Constraint {
public:
  using VectorXd = Eigen::VectorXd;
  using Jacobian = Eigen::SparseMatrix<double, Eigen::RowMajor>;
  using ParametrizationPtr = std::shared_ptr<Parametrization>;
  using VarPair = std::pair<ParametrizationPtr,Jacobian>;

  Constraint ();
  virtual ~Constraint ();

  /** @brief Sets the values stored in variables_ to the current NLP ones
    */
  void UpdateVariables(const OptimizationVariables*);

  /** @brief Jacobian of the constraints with respect to each decision variable set
    */
  Jacobian GetJacobianWithRespectTo (std::string var_set) const;

  /** @brief A constraint always delivers a vector of constraint violations.
   */
  VectorXd GetConstraintValues() const;

  /** @brief For each returned constraint an upper and lower bound is given.
    */
  VecBound GetBounds() const;

  void PrintStatus(double tol) const;
  int GetNumberOfConstraints() const;

protected:
  /** @brief The values of these variables influence the constraint.
    *
    * Subscribes to these values and keeps them up-to-date to be used to
    * calculate the constraints and bounds.
    */
  void SetDependentVariables(const std::vector<ParametrizationPtr>&,
                             int num_constraints);

  /** @returns a writable reference to modify the Jacobian of the constraint.
    *
    * @param var_set The differentiation of the constraint w.r.t these variables
    *                produces this Jacobian.
   */
  Jacobian& GetJacobianRefWithRespectTo (std::string var_set);

  std::string name_;
  VectorXd g_;
  VecBound bounds_;

private:
  /** @brief A constraint always delivers a vector of constraint violations.
    *
    * This is specific to each type of constraint and must be implemented
    * by the user.
    */
  virtual void UpdateConstraintValues () = 0;

  /** @brief For each returned constraint an upper and lower bound is given.
    *
    * This is specific to each type of constraint and must be implemented
    * by the user.
    */
  virtual void UpdateBounds () = 0;

  /** @brief Implement this if the Jacobians change with different values of the
    * optimization variables, so are not constant.
    */
  virtual void UpdateJacobians() {/* do nothing assuming Jacobians constant */};

  std::vector<VarPair> variables_;
  int num_constraints_ = 0;
};

} /* namespace opt */
} /* namespace xpp */

#endif /* XPP_XPP_OPT_INCLUDE_XPP_OPT_CONSTRAINT_H_ */