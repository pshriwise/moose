//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#pragma once

#include "ADStressUpdateBase.h"
#include "ADSingleVariableReturnMappingSolution.h"

#define usingViscoplasticityStressUpdateBaseMembers                                                \
  usingStressUpdateBaseMembers;                                                                    \
  usingSingleVariableReturnMappingSolutionMembers;                                                 \
  using ADViscoplasticityStressUpdateBase<compute_stage>::updateIntermediatePorosity;              \
  using ADViscoplasticityStressUpdateBase<compute_stage>::computeStressInitialize;                 \
  using ADViscoplasticityStressUpdateBase<compute_stage>::computeStressFinalize;                   \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_intermediate_porosity;                  \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_derivative;                             \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_effective_inelastic_strain;             \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_effective_inelastic_strain_old;         \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_inelastic_strain;                       \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_inelastic_strain_old;                   \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_verbose;                                \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_porosity_old;                           \
  using ADViscoplasticityStressUpdateBase<compute_stage>::_max_inelastic_increment

template <ComputeStage>
class ADViscoplasticityStressUpdateBase;

declareADValidParams(ADViscoplasticityStressUpdateBase);

template <ComputeStage compute_stage>
class ADViscoplasticityStressUpdateBase
  : public ADStressUpdateBase<compute_stage>,
    public ADSingleVariableReturnMappingSolution<compute_stage>
{
public:
  static InputParameters validParams();

  ADViscoplasticityStressUpdateBase(const InputParameters & parameters);

  virtual Real computeReferenceResidual(const ADReal & effective_trial_stress,
                                        const ADReal & scalar_effective_inelastic_strain) override;

  /**
   * Compute the limiting value of the time step for this material
   * @return Limiting time step
   */
  virtual Real computeTimeStepLimit() override;

  /**
   * Does the model require the elasticity tensor to be isotropic?
   */
  bool requiresIsotropicTensor() override { return true; }

protected:
  virtual void initQpStatefulProperties() override;

  virtual void propagateQpStatefulProperties() override;

  /**
   * Perform any necessary initialization before return mapping iterations
   * @param effective_trial_stress Effective trial stress
   * @param elasticityTensor     Elasticity tensor
   */
  virtual void computeStressInitialize(const ADReal & /*effective_trial_stress*/,
                                       const ADRankFourTensor & /*elasticity_tensor*/)
  {
  }

  /**
   * Perform any necessary steps to finalize state after return mapping iterations
   * @param inelasticStrainIncrement Inelastic strain increment
   */
  virtual void computeStressFinalize(const ADRankTwoTensor & /*plastic_strain_increment*/) {}

  virtual ADReal computeDerivative(const ADReal & /*effective_trial_stress*/,
                                   const ADReal & /*scalar*/) override
  {
    return _derivative;
  }

  void updateIntermediatePorosity(const ADRankTwoTensor & elastic_strain_increment);

  void outputIterationSummary(std::stringstream * iter_output,
                              const unsigned int total_it) override;

  /// String designating the base name of the total strain
  const std::string _total_strain_base_name;

  /// Material property for the total strain increment
  const ADMaterialProperty(RankTwoTensor) & _strain_increment;

  ///@{ Effective inelastic strain material property
  ADMaterialProperty(Real) & _effective_inelastic_strain;
  const MaterialProperty<Real> & _effective_inelastic_strain_old;
  ///@}

  ///@{ Creep strain material property
  ADMaterialProperty(RankTwoTensor) & _inelastic_strain;
  const MaterialProperty<RankTwoTensor> & _inelastic_strain_old;
  ///@}

  /// Max increment for inelastic strain
  Real _max_inelastic_increment;

  /// Container for the porosity calculated from all other intelastic models except the current model
  ADReal _intermediate_porosity;

  /// Material property for the old porosity
  const MaterialProperty<Real> & _porosity_old;

  /// Flag to enable verbose output
  const bool _verbose;

  /// Container for _derivative
  ADReal _derivative;

  usingStressUpdateBaseMembers;
  usingSingleVariableReturnMappingSolutionMembers;
};
