/*
 * Copyright (c) 2009--2018, the KLFitter developer team
 *
 * This file is part of KLFitter.
 *
 * KLFitter is free software: you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 *
 * KLFitter is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with KLFitter. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KLFITTER_LIKELIHOODTOPALLHADRONIC_H_
#define KLFITTER_LIKELIHOODTOPALLHADRONIC_H_

#include <cmath>
#include <iostream>
#include <vector>

namespace KLFitter {
  class ResolutionBase;
}

#include "KLFitter/LikelihoodBase.h"

// ---------------------------------------------------------

/**
 * \namespace KLFitter
 * \brief The KLFitter namespace
 */
namespace KLFitter {
/**
  * \class KLFitter::LikelihoodTopAllHadronic
  * \brief A class implementing a likelihood for the ttbar allhadronic channel.
  *
  * This class represents a likelihood for the ttbar allhadronic channel.
  */
class LikelihoodTopAllHadronic : public KLFitter::LikelihoodBase {
 public:
  /** \name Constructors and destructors */
  /* @{ */

  /**
    * The default constructor.
    */
  LikelihoodTopAllHadronic();

  /**
    * The default destructor.
    */
  ~LikelihoodTopAllHadronic();

  /* @} */
  /** \name Member functions (Get)  */
  /* @{ */

  /* @} */
  /** \name Member functions (Set)  */
  /* @{ */

  /**
    * Enumerator for the parameters.
    */
  enum Parameters { parBhad1E, parBhad2E, parLQ1E, parLQ2E, parLQ3E, parLQ4E, parTopM };

  /**
    * Set a flag. If flag is true the invariant top quark mass is
    * fixed to the pole mass.
    * @param flag The flag.
    */
  void SetFlagTopMassFixed(bool flag) { fFlagTopMassFixed = flag; }

  void SetFlagUseJetMass(bool flag) { fFlagUseJetMass = flag; }

  void SetFlagGetParSigmasFromTFs(bool flag) { fFlagGetParSigmasFromTFs = flag; }

  /* @} */
  /** \name Member functions (misc)  */
  /* @{ */

  /* @} */
  /** \name Member functions (BAT)  */
  /* @{ */

  /**
    * Define the parameters of the fit.
    */
  void DefineParameters() override;

  /**
    * The prior probability definition, overloaded from BCModel.
    * @param parameters A vector of parameters (double values).
    * @return The logarithm of the prior probability.
    */
  double LogAPrioriProbability(const std::vector <double> & parameters) override { return 0; }

  /**
    * The posterior probability definition, overloaded from BCModel.
    * @param parameters A vector of parameters (double values).
    * @return The logarithm of the prior probability.
    */
  double LogLikelihood(const std::vector <double> &  parameters) override;

  /**
    * The posterior probability definition, overloaded from BCModel. Split up into several subcomponents
    * @param parameters A vector of parameters (double values).
    * @return A vector with the components of the logarithm of the prior probability. Its components are:
    * 0:  TF_bhad1
    * 1:  TF_bhad2
    * 2:  TF_lq1
    * 3:  TF_lq2
    * 4:  TF_lq3
    * 5:  TF_lq4
    * 6:  BW_Whad1
    * 7:  BW_Whad2
    * 8:  BW_Thad1
    * 9:  BW_Thad2
    */
  std::vector<double> LogLikelihoodComponents(std::vector <double> parameters) override;

  /**
    * Get initial values for the parameters.
    * @return vector of initial values.
    */
  std::vector<double> GetInitialParameters() override;

  /**
    * Check if there are TF problems.
    * @return Return false if TF problem.
    */
  bool NoTFProblem(std::vector<double> parameters) override;

  /**
    * Return the set of model particles.
    * @return A pointer to the particles.
    */
  KLFitter::Particles* ParticlesModel() override {
    BuildModelParticles();
    return fParticlesModel;
  }
  KLFitter::Particles** PParticlesModel() override {
    BuildModelParticles();
    return &fParticlesModel;
  }

  /* @} */

 protected:
  /** \name Member functions (misc)  */
  /* @{ */

  /**
    * Update 4-vectors of model particles.
    * @return An error flag.
    */
  int CalculateLorentzVectors(std::vector <double> const& parameters);

  /**
    * Initialize the likelihood for the event
    */
  int Initialize() override;

  /**
    * Adjust parameter ranges
    */
  int AdjustParameterRanges();

  /**
    * Define the model particles
    * @return An error code.
    */
  int DefineModelParticles();

  /**
    * Remove invariant particle permutations.
    * @return An error code.
    */
  int RemoveInvariantParticlePermutations() override;

  /**
    * Remove forbidden particle permutations.
    * @return An error code.
    */
  int RemoveForbiddenParticlePermutations() override;

  /**
    * Remove forbidden particle permutations - forcing b-jets on the position of a b parton.
    * @return An error code.
    */
  //    int RemoveForbiddenBParticlePermutations();

  /**
    * Build the model particles from the best fit parameters.
    * @return An error code.
    */
  int BuildModelParticles();

  /* @} */

 protected:
  /**
    * A flag for using a fixed top mass (true) or not (false).
    */
  bool fFlagTopMassFixed;

  /**
    * A flag for using the measured jet masses (true) instead of
    * parton masses (false);
    */
  bool fFlagUseJetMass;

  /**
    *  Flag for using ResolutionBase::GetSigma() to retrieve the parameter ranges
    */
  bool fFlagGetParSigmasFromTFs;

  /**
    * Save permuted particles.
    */
  int SavePermutedParticles();

  /**
    * Save resolution functions.
    */
  int SaveResolutionFunctions();

  /**
    * Set model parton mass according to fFlagUseJetMass.
    * @param jetmass The jet mass.
    * @param quarkmass The quark mass.
    * @param px The parton px (will be modified, if necessary).
    * @param py The parton py (will be modified, if necessary).
    * @param pz The parton pz (will be modified, if necessary).
    * @param e The parton energy (not modified).
    * @return The parton mass.
    */
  double SetPartonMass(double jetmass, double quarkmass, double *px, double *py, double *pz, double e) {
    double mass(0.);
    if (fFlagUseJetMass) {
      mass = jetmass > 0. ? jetmass : 0.;
    } else {
      mass = quarkmass;
    }
    double p_orig = sqrt(*px * *px + *py * *py + *pz * *pz);
    double p_newmass = sqrt(e * e - mass * mass);
    double scale = p_newmass / p_orig;
    *px *= scale;
    *py *= scale;
    *pz *= scale;
    return mass;
  }

  /**
    * Global variable for TF problems.
    */
  bool fTFgood;

  /**
    * Save resolution functions since the eta of the partons is not fitted.
    */
  ResolutionBase * fResEnergyBhad1;
  ResolutionBase * fResEnergyBhad2;
  ResolutionBase * fResEnergyLQ1;
  ResolutionBase * fResEnergyLQ2;
  ResolutionBase * fResEnergyLQ3;
  ResolutionBase * fResEnergyLQ4;

  /**
    * Save measured particle values for frequent calls
    */
  double bhad1_meas_e;
  double bhad1_meas_p;
  double bhad1_meas_m;
  double bhad1_meas_deteta;
  double bhad1_meas_eta;
  double bhad1_meas_phi;
  double bhad1_meas_px;
  double bhad1_meas_py;
  double bhad1_meas_pz;

  double bhad2_meas_e;
  double bhad2_meas_p;
  double bhad2_meas_m;
  double bhad2_meas_deteta;
  double bhad2_meas_eta;
  double bhad2_meas_phi;
  double bhad2_meas_px;
  double bhad2_meas_py;
  double bhad2_meas_pz;

  double lq1_meas_e;
  double lq1_meas_p;
  double lq1_meas_m;
  double lq1_meas_deteta;
  double lq1_meas_eta;
  double lq1_meas_phi;
  double lq1_meas_px;
  double lq1_meas_py;
  double lq1_meas_pz;

  double lq2_meas_e;
  double lq2_meas_p;
  double lq2_meas_m;
  double lq2_meas_deteta;
  double lq2_meas_eta;
  double lq2_meas_phi;
  double lq2_meas_px;
  double lq2_meas_py;
  double lq2_meas_pz;

  double lq3_meas_e;
  double lq3_meas_p;
  double lq3_meas_m;
  double lq3_meas_deteta;
  double lq3_meas_eta;
  double lq3_meas_phi;
  double lq3_meas_px;
  double lq3_meas_py;
  double lq3_meas_pz;

  double lq4_meas_e;
  double lq4_meas_p;
  double lq4_meas_m;
  double lq4_meas_deteta;
  double lq4_meas_eta;
  double lq4_meas_phi;
  double lq4_meas_px;
  double lq4_meas_py;
  double lq4_meas_pz;

  /**
    * Save fit particle values for frequent calls
    */
  double bhad1_fit_e;
  double bhad1_fit_px;
  double bhad1_fit_py;
  double bhad1_fit_pz;

  double bhad2_fit_e;
  double bhad2_fit_px;
  double bhad2_fit_py;
  double bhad2_fit_pz;

  double lq1_fit_e;
  double lq1_fit_px;
  double lq1_fit_py;
  double lq1_fit_pz;

  double lq2_fit_e;
  double lq2_fit_px;
  double lq2_fit_py;
  double lq2_fit_pz;

  double lq3_fit_e;
  double lq3_fit_px;
  double lq3_fit_py;
  double lq3_fit_pz;

  double lq4_fit_e;
  double lq4_fit_px;
  double lq4_fit_py;
  double lq4_fit_pz;

  double whad1_fit_m;
  double whad2_fit_m;
  double thad1_fit_m;
  double thad2_fit_m;
};
}  // namespace KLFitter

#endif  // KLFITTER_LIKELIHOODTOPALLHADRONIC_H_
