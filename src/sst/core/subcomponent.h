// Copyright 2009-2019 NTESS. Under the terms
// of Contract DE-NA0003525 with NTESS, the U.S.
// Government retains certain rights in this software.
// 
// Copyright (c) 2009-2019, NTESS
// All rights reserved.
// 
// This file is part of the SST software package. For license
// information, see the LICENSE file in the top level directory of the
// distribution.


#ifndef SST_CORE_SUBCOMPONENT_H
#define SST_CORE_SUBCOMPONENT_H

#include <sst/core/warnmacros.h>
#include <sst/core/baseComponent.h>
#include <sst/core/component.h>
#include <sst/core/module.h>
#include <sst/core/eli/elementinfo.h>

namespace SST {

/**
   SubComponent is a class loadable through the factory which allows
   dynamic functionality to be added to a Component.  The
   SubComponent API is nearly identical to the Component API and all
   the calls are proxied to the parent Component.
*/
class SubComponent : public Module, public BaseComponent {

public:
    SST_ELI_DECLARE_BASE(SubComponent)
    //declare extern to limit compile times
    SST_ELI_DECLARE_CTOR_EXTERN(Component*,SST::Params&)
    SST_ELI_DECLARE_INFO_EXTERN( 
      ELI::ProvidesParams,
      ELI::ProvidesSubComponentSlots,
      ELI::ProvidesPorts,
      ELI::ProvidesStats,
      ELI::ProvidesInterface)

	SubComponent(Component* parent);
	SubComponent(ComponentId_t id);

	virtual ~SubComponent() {};

    /** Used during the init phase.  The method will be called each phase of initialization.
     Initialization ends when no components have sent any data. */
    virtual void init(unsigned int UNUSED(phase)) override {}
    /** Called after all components have been constructed and initialization has
	completed, but before simulation time has begun. */
    virtual void setup( ) override { }
    /** Called after simulation completes, but before objects are
        destroyed. A good place to print out statistics. */
    virtual void finish( ) override { }

protected:
    Component* const parent __attribute__ ((deprecated("The parent data member will be removed in SST version 10.0.  With the new subcomponent structure, direct access to your parent is not allowed.")));

    /* Deprecate?   Old ELI style*/
    SubComponent* loadSubComponent(std::string type, Params& params) __attribute__ ((deprecated("This version of loadSubComponent will be removed in SST version 10.0.  Please switch to new user defined API (LoadUserSubComponent(std::string, int, ARGS...)).")));


private:
    friend class Component;

};

} //namespace SST


// Legacy version of subcomponent registration
#define SST_ELI_REGISTER_SUBCOMPONENT(cls,lib,name,version,desc,interface)   \
    SST_ELI_REGISTER_DERIVED(SST::SubComponent,cls,lib,name,ELI_FORWARD_AS_ONE(version),desc) \
    SST_ELI_INTERFACE_INFO(interface)

// New way to register subcomponents.  Must register an interface
// (API) first, then you can register a subcomponent that implements
// it
#define SST_ELI_REGISTER_SUBCOMPONENT_API(cls,...)              \
    SST_ELI_DECLARE_NEW_BASE(SST::SubComponent,::cls)           \
    SST_ELI_NEW_BASE_CTOR(ComponentId_t,Params&,##__VA_ARGS__)

#define SST_ELI_REGISTER_SUBCOMPONENT_DERIVED_API(cls,base,...) \
    SST_ELI_DECLARE_NEW_BASE(::base,::cls)                          \
    SST_ELI_NEW_BASE_CTOR(ComponentId_t,Params&,##__VA_ARGS__)

#define SST_ELI_REGISTER_SUBCOMPONENT_DERIVED(cls,lib,name,version,desc,interface)   \
    SST_ELI_REGISTER_DERIVED(::interface,cls,lib,name,ELI_FORWARD_AS_ONE(version),desc) \
    SST_ELI_INTERFACE_INFO(#interface)

#endif // SST_CORE_SUBCOMPONENT_H
