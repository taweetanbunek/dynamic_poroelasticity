#ifndef MOONEY_CC_H_
#define MOONEY_CC_H_

#include <dense_vector.h>
#include <dense_matrix.h>
#include <vector_value.h>
#include <tensor_value.h>
#include <getpot.h>

#include "defines.h"

#if MOONEY


#if COMPRESSIBLE && ! PORO
class MooneyCurrentConfig {
public:
  MooneyCurrentConfig(
      const std::vector<std::vector<RealGradient> >& dphi) :
      dphi(dphi) {
    E = 0.1;
    nu = 0.3;
   K1=10000;
  K2=10;
  }
#endif

#if INCOMPRESSIBLE || (CHAP) 
class MooneyCurrentConfig {
public:
  MooneyCurrentConfig(
      const std::vector<std::vector<RealGradient> >& dphi,   const std::vector<std::vector<Real> >& psi) :
      dphi(dphi),psi(psi) {
   E = 1;
    nu = 0.3;
    // mu is the shear modulus is one of several quantities for measuring the stiffness of materials
    //Polyethylene (plastic)  mu= 0.117
    ///Rubber mu=  0.0006
 K1=10;
  K2=10;
   
    

  }

#endif

 

  /**
   * Initialize the class for the given displacement gradient at the
   * specified quadrature point.
   */
  void init_for_qp(VectorValue<Gradient> & grad_u, Number & p_current, unsigned int qp);
  void init_for_qp(VectorValue<Gradient> & grad_u, Number & p_current, unsigned int qp, Real m);

  /**
   * Return the residual vector for the current state.
   */

  void get_residual(DenseVector<Real> & residuum, unsigned int & i);

  /**
   * Return the stiffness matrix for the current state.
   */
  void get_linearized_stiffness(DenseMatrix<Real> & stiffness,
      unsigned int & i, unsigned int & j);

  #if INCOMPRESSIBLE || (CHAP) 
  void get_linearized_p_uvw_stiffness(DenseVector<Real> & p_stiffness, unsigned int & i, unsigned int & j);
  void get_linearized_uvw_p_stiffness(DenseVector<Real> & p_stiffness, unsigned int & i, unsigned int & j);
  void get_p_residual(DenseVector<Real> & p_residuum, unsigned int & i);
  #endif
  
  /**
   * Flag to indicate if it is necessary to calculate values for stiffness
   * matrix during initialization.
   */
  bool calculate_linearized_stiffness;
//private:
  void build_b_0_mat(int i, DenseMatrix<Real>& b_l_mat);

  void calculate_stress();
  void calculate_tangent();
  static void tensor_to_voigt(const RealTensor &tensor, DenseVector<Real> &vec);
  static void tensorOtensor_to_voigt(const RealTensor &tensorB, const RealTensor &tensorA, DenseMatrix<Real> &mat);
    static void z_ref_to_voigt(const RealTensor &tensorB, const RealTensor &tensorA, DenseMatrix<Real> &mat);

void c_update(RealTensor C);

  unsigned int current_qp;
  const std::vector<std::vector<RealGradient> >& dphi;

Real K1 ;
Real K2 ;
Real m;
  Real p_solid;

RealTensor Identity;
Real I_1;
Real I_2;
Real I_3;
Real J;
Real f ;
Real gamma; 




  DenseMatrix<Real> C_mat;
  Real E;
  Real nu;

  
  #if INCOMPRESSIBLE || (CHAP)  
  Number p_current;
 // Real detF;
  //RealTensor invF;
  const std::vector<std::vector<Real> >& psi;
  #endif     
  
  RealTensor F, S, tau, sigma, C, invC, b, Ft;
  DenseMatrix<Real> B_L;
  DenseMatrix<Real> B_K;
};


#endif
#endif /* Mooney_CC_H_ */

