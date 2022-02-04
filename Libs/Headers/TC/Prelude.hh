/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2021, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <TC/Assertion.hh>
#include <TC/Collection/LinearAtIterator.hh>
#include <TC/Collection/List.hh>
#include <TC/Collection/Map.hh>
#include <TC/Collection/NestedIterator.hh>
#include <TC/Collection/Pair.hh>
#include <TC/Collection/Vector.hh>
#include <TC/Cxx/Exchange.hh>
#include <TC/Cxx/Forward.hh>
#include <TC/Cxx/Move.hh>
#include <TC/Cxx/New.hh>
#include <TC/Cxx/NullPtr.hh>
#include <TC/Cxx/Swap.hh>
#include <TC/Functional/ErrorOr.hh>
#include <TC/Functional/Must.hh>
#include <TC/Functional/Option.hh>
#include <TC/Functional/Result.hh>
#include <TC/Functional/Try.hh>
#include <TC/Hashing/Integer.hh>
#include <TC/Hashing/Pointer.hh>
#include <TC/IntTypes.hh>
#include <TC/RawMemory.hh>
#include <TC/Tag/Adopt.hh>
#include <TC/Tag/Copy.hh>
#include <TC/Tag/None.hh>
#include <TC/Tag/Wrap.hh>
#include <TC/Trait/AddConst.hh>
#include <TC/Trait/AddVolatile.hh>
#include <TC/Trait/Conditional.hh>
#include <TC/Trait/Constant.hh>
#include <TC/Trait/CopyConst.hh>
#include <TC/Trait/EnableIf.hh>
#include <TC/Trait/FalseType.hh>
#include <TC/Trait/First.hh>
#include <TC/Trait/In.hh>
#include <TC/Trait/IndexOf.hh>
#include <TC/Trait/IsBaseOf.hh>
#include <TC/Trait/IsClass.hh>
#include <TC/Trait/IsConst.hh>
#include <TC/Trait/IsFloatingPoint.hh>
#include <TC/Trait/IsFunction.hh>
#include <TC/Trait/IsIntegral.hh>
#include <TC/Trait/IsLValue.hh>
#include <TC/Trait/IsPointer.hh>
#include <TC/Trait/IsRValue.hh>
#include <TC/Trait/IsSame.hh>
#include <TC/Trait/IsUnion.hh>
#include <TC/Trait/IsVoid.hh>
#include <TC/Trait/MakeSigned.hh>
#include <TC/Trait/MakeUnsigned.hh>
#include <TC/Trait/Max.hh>
#include <TC/Trait/RemoveConst.hh>
#include <TC/Trait/RemoveConstVolatile.hh>
#include <TC/Trait/RemovePointer.hh>
#include <TC/Trait/RemoveReference.hh>
#include <TC/Trait/RemoveVolatile.hh>
#include <TC/Trait/TrueType.hh>
#include <TC/Trait/TypeIntrinsics.hh>
