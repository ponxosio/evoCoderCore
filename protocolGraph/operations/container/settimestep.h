#ifndef SETTIMESTEP_H
#define SETTIMESTEP_H

//local
#include "operables/mathematics/MathematicOperable.h"
#include "ContainerOperation.h"
#include "util/Patch.h"

//cereal
#include <cereal/cereal.hpp>
#include <cereal/types/memory.hpp>
#include <cereal/types/polymorphic.hpp>

#include "evocodercore_global.h"

class SETTIMESTEP_EXPORT SetTimeStep: public ContainerOperation {
public:
    // Node methods
    SetTimeStep();
    SetTimeStep(const SetTimeStep & node);

    virtual std::string toText();
    virtual void loadNode(const std::string & line) throw (invalid_argument);
    //
    SetTimeStep(int containerId, std::shared_ptr<MathematicOperable> timeSlice);

    virtual ~SetTimeStep();

    virtual void execute() throw(std::invalid_argument);

    virtual void updateReference(const std::string & reference);

    //SERIALIZATIoN
    template<class Archive>
    void serialize(Archive & ar, std::uint32_t const version);

protected:
    std::shared_ptr<MathematicOperable> timeSlice;
};

template<class Archive>
inline void SetTimeStep::serialize(Archive& ar, const std::uint32_t version) {
    if (version <= 1) {
        ContainerOperation::serialize(ar, version);
        ar(CEREAL_NVP(timeSlice));
    }
}

// Associate some type with a version number
CEREAL_CLASS_VERSION( SetTimeStep, (int)1 );

// Include any archives you plan on using with your type before you register it
// Note that this could be done in any other location so long as it was prior
// to this file being included
#include <cereal/archives/json.hpp>
// Register DerivedClass
CEREAL_REGISTER_TYPE_WITH_NAME(SetTimeStep,"SetTimeStep");

#endif // SETTIMESTEP_H
