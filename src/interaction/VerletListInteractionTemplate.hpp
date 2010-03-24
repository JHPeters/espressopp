#ifndef _INTERACTION_VERLETLISTINTERACTIONTEMPLATE_HPP
#define _INTERACTION_VERLETLISTINTERACTIONTEMPLATE_HPP

#include "Interaction.hpp"
#include "Real3D.hpp"
#include "Particle.hpp"
#include "VerletList.hpp"

namespace espresso {
  namespace interaction {
    template < typename _Potential >
    class VerletListInteractionTemplate
        : public Interaction {
    protected:
      typedef _Potential Potential;
      typedef VerletListInteractionTemplate< Potential> Super;
    public:
      VerletListInteractionTemplate
      (shared_ptr < VerletList > _verletList)
        : verletList(_verletList) 
      {}

      void
      setVerletList(shared_ptr < VerletList > _verletList) {
        verletList = _verletList;
      }

      shared_ptr < VerletList > getVerletList() {
        return verletList;
      }

      void
      setPotential(int type1, int type2, const Potential &potential) {
        // TODO: automatically resize array
	//        potentialArray[type1][type2] = potential;
	potentialArray = potential;
      }

      Potential &getPotential(int type1, int type2) {
        // TODO: automatically resize array
	//        return potentialArray[type1][type2];
        return potentialArray;
      }

      virtual void addForces();
      virtual real computeEnergy();
      virtual real getMaxCutoff();

    protected:
      int ntypes;
      shared_ptr < VerletList > verletList;
      Potential potentialArray;
    };

    //////////////////////////////////////////////////
    // INLINE IMPLEMENTATION
    //////////////////////////////////////////////////
    template < typename _Potential > inline void
    VerletListInteractionTemplate < _Potential >::addForces() {
      LOG4ESPP_INFO(theLogger, "add forces computed by the Verlet List");
      const PairList pairList = verletList->getPairs();

      int npairs = pairList.size();
      for (int i = 0; i < npairs; i++) {
        Particle &p1 = *pairList[i].first;
        Particle &p2 = *pairList[i].second;
        int type1 = p1.p.type;
        int type2 = p2.p.type;
        const Potential &potential = getPotential(type1, type2);

	Real3D force;
	if (potential._computeForce(p1, p2, force))
	  for(int k = 0; k < 3; k++) {
	    p1.f.f[k] += force[k];
	    p2.f.f[k] -= force[k];
	  }
	}
#if 0
          printf
          ("dist(%d,%d), dist = %f -> %f %f %f\n",
           p1.p.id, p2.p.id, distSqr, dist[0], dist[1], dist[2]);
          printf
          ("force(%d,%d), dist = %f -> %f %f %f\n",
           p1.p.id, p2.p.id, distSqr,
           force[0], force[1], force[2]);
          if(p1.p.id == 0) {
            printf
            ("sum add force Particle 0 = %f %f %f\n",
             p1.f.f[0], p1.f.f[1], p1.f.f[0]);
          }
          if(p2.p.id == 0) {
            printf
            ("sum sub force Particle 0 = %f %f %f\n",
             p2.f.f[0], p2.f.f[1], p2.f.f[0]);
          }
#endif
    }
    
    template < typename _Potential >
    inline real
    VerletListInteractionTemplate < _Potential >::
    computeEnergy() {
      LOG4ESPP_INFO(theLogger, "compute energy by the Verlet List");

      const PairList pairList = verletList->getPairs();

      int npairs = pairList.size();
      real e = 0.0;
      for(int i = 0; i < npairs; i++) {
        Particle &p1 = *pairList[i].first;
        Particle &p2 = *pairList[i].second;
        int type1 = p1.p.type;
        int type2 = p2.p.type;
        const Potential &potential = getPotential(type1, type2);
	e += potential._computeEnergy(p1, p2);
      }
      return e;
    }

    template < typename _Potential >
    inline real
    VerletListInteractionTemplate <
    _Potential >::getMaxCutoff() {
      real cutoff = 0.0;

      for(int i = 0; i < ntypes; i++) {
        for(int j = 0; i < ntypes; i++) {
          cutoff = std::max(cutoff, getPotential(i, j).getCutoff());
        }
      }
      return cutoff;
    }
  }
}
#endif