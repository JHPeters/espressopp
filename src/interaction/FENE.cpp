#include "python.hpp"
#include "FENE.hpp"

namespace espresso {
  namespace interaction {
    //////////////////////////////////////////////////
    // REGISTRATION WITH PYTHON
    //////////////////////////////////////////////////
    void 
    FENE::registerPython() {
      using namespace espresso::python;

      class_< FENE, bases< Potential > >
    	("interaction_FENE", init< real, real, real, real >())
	.def(init< real, real, real, real, real >())
	.add_property("K", &FENE::getK, &FENE::setK)
	.add_property("r0", &FENE::getR0, &FENE::setR0)
	.add_property("rMax", &FENE::getRMax, &FENE::setRMax)
    	;

    }

  }
}