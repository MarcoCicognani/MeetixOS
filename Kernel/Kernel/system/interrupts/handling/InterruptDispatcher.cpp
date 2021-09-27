/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * * *
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                                    *
 *                                                                                     *
 *         DERIVED FROM THE GHOST OPERATING SYSTEM                                     *
 *         This software is derived from the Ghost operating system project,           *
 *         written by Max Schlüssel <lokoxe@gmail.com>. Copyright 2012-2017            *
 *         https://ghostkernel.org/                                                    *
 *         https://github.com/maxdev1/ghost                                            *
 *                                                                                     *
 * This program is free software; you can redistribute it and/or                       *
 * modify it under the terms of the GNU General Public License                         *
 * as published by the Free Software Foundation; either version 2                      *
 * of the License, or (char *argumentat your option) any later version.                *
 *                                                                                     *
 * This program is distributed in the hope that it will be useful,                     *
 * but WITHout ANY WARRANTY; without even the implied warranty of                      *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the                       *
 * GNU General Public License for more details.                                        *
 *                                                                                     *
 * You should have received a copy of the GNU General Public License                   *
 * along with this program; if not, write to the Free Software                         *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA      *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *  * * * * * * */

#include <EvangelionNG.hpp>
#include <logger/logger.hpp>
#include <system/interrupts/descriptors/idt.hpp>
#include <system/interrupts/handling/InterruptDispatcher.hpp>
#include <system/interrupts/handling/InterruptExceptionHandler.hpp>
#include <system/interrupts/handling/InterruptRequestHandler.hpp>
#include <system/interrupts/handling/InterruptStubs.hpp>
#include <system/interrupts/lapic.hpp>
#include <system/interrupts/pic.hpp>
#include <system/IOPorts.hpp>
#include <system/ProcessorState.hpp>
#include <system/smp/GlobalLock.hpp>
#include <tasking/tasking.hpp>
#include <tasking/wait/waiter.hpp>

/**
 * Interrupt handler routine, called by the interrupt stubs (assembly file)
 *
 * @param cpuState:		the current CPU state
 * @return the CPU state to be applied
 */
extern "C" ProcessorState* _interruptHandler(ProcessorState* cpuState) {
    return InterruptDispatcher::handle(cpuState);
}

/**
 * locker for handling, when an interrupt is called
 * the other cores (on AP) can't call the kernel
 */
static GlobalLock handlingLock;

/**
 * Dispatches the interrupt handling and EOIs.
 *
 * @param state:		the current task's cpu state
 * @return the new cpu state to be setted
 */
ProcessorState* InterruptDispatcher::handle(ProcessorState* cpuState) {
    // global system lock
    handlingLock.lock();

    // save current task state
    auto currentThread = Tasking::save(cpuState);

    // Exceptions (interrupts below 0x20) are redirected to the exception handler,
    // while requests are redirected to the request handler.
    if ( cpuState->intr < 0x20 ) {
        currentThread = InterruptExceptionHandler::handle(currentThread);
    } else {
        currentThread = InterruptRequestHandler::handle(currentThread);
    }

    // sanity check
    if ( currentThread->waitManager ) {
        EvaKernel::panic("scheduled thread %i had a wait manager ('%s')",
                         currentThread->id,
                         currentThread->waitManager->debugName());
    }

    // send end of interrupt
    Lapic::sendEoi();

    // global system unlock
    handlingLock.unlock();

    return currentThread->cpuState;
}

/**
 * Installs the interrupt stubs to the IDT.
 */
void InterruptDispatcher::install() {
    Idt::createGate(0, (uint32_t)_irout0, 0x08, 0x8E);
    Idt::createGate(1, (uint32_t)_irout1, 0x08, 0x8E);
    Idt::createGate(2, (uint32_t)_irout2, 0x08, 0x8E);
    Idt::createGate(3, (uint32_t)_irout3, 0x08, 0x8E);
    Idt::createGate(4, (uint32_t)_irout4, 0x08, 0x8E);
    Idt::createGate(5, (uint32_t)_irout5, 0x08, 0x8E);
    Idt::createGate(6, (uint32_t)_irout6, 0x08, 0x8E);
    Idt::createGate(7, (uint32_t)_irout7, 0x08, 0x8E);
    Idt::createGate(8, (uint32_t)_irout8, 0x08, 0x8E);
    Idt::createGate(9, (uint32_t)_irout9, 0x08, 0x8E);
    Idt::createGate(10, (uint32_t)_irout10, 0x08, 0x8E);
    Idt::createGate(11, (uint32_t)_irout11, 0x08, 0x8E);
    Idt::createGate(12, (uint32_t)_irout12, 0x08, 0x8E);
    Idt::createGate(13, (uint32_t)_irout13, 0x08, 0x8E);
    Idt::createGate(14, (uint32_t)_irout14, 0x08, 0x8E);
    Idt::createGate(15, (uint32_t)_irout15, 0x08, 0x8E);
    Idt::createGate(16, (uint32_t)_irout16, 0x08, 0x8E);
    Idt::createGate(17, (uint32_t)_irout17, 0x08, 0x8E);
    Idt::createGate(18, (uint32_t)_irout18, 0x08, 0x8E);
    Idt::createGate(19, (uint32_t)_irout19, 0x08, 0x8E);
    Idt::createGate(20, (uint32_t)_irout20, 0x08, 0x8E);
    Idt::createGate(21, (uint32_t)_irout21, 0x08, 0x8E);
    Idt::createGate(22, (uint32_t)_irout22, 0x08, 0x8E);
    Idt::createGate(23, (uint32_t)_irout23, 0x08, 0x8E);
    Idt::createGate(24, (uint32_t)_irout24, 0x08, 0x8E);
    Idt::createGate(25, (uint32_t)_irout25, 0x08, 0x8E);
    Idt::createGate(26, (uint32_t)_irout26, 0x08, 0x8E);
    Idt::createGate(27, (uint32_t)_irout27, 0x08, 0x8E);
    Idt::createGate(28, (uint32_t)_irout28, 0x08, 0x8E);
    Idt::createGate(29, (uint32_t)_irout29, 0x08, 0x8E);
    Idt::createGate(30, (uint32_t)_irout30, 0x08, 0x8E);
    Idt::createGate(31, (uint32_t)_irout31, 0x08, 0x8E);

    Idt::createGate(32, (uint32_t)_ireq0, 0x08, 0x8E);
    Idt::createGate(33, (uint32_t)_ireq1, 0x08, 0x8E);
    Idt::createGate(34, (uint32_t)_ireq2, 0x08, 0x8E);
    Idt::createGate(35, (uint32_t)_ireq3, 0x08, 0x8E);
    Idt::createGate(36, (uint32_t)_ireq4, 0x08, 0x8E);
    Idt::createGate(37, (uint32_t)_ireq5, 0x08, 0x8E);
    Idt::createGate(38, (uint32_t)_ireq6, 0x08, 0x8E);
    Idt::createGate(39, (uint32_t)_ireq7, 0x08, 0x8E);
    Idt::createGate(40, (uint32_t)_ireq8, 0x08, 0x8E);
    Idt::createGate(41, (uint32_t)_ireq9, 0x08, 0x8E);
    Idt::createGate(42, (uint32_t)_ireq10, 0x08, 0x8E);
    Idt::createGate(43, (uint32_t)_ireq11, 0x08, 0x8E);
    Idt::createGate(44, (uint32_t)_ireq12, 0x08, 0x8E);
    Idt::createGate(45, (uint32_t)_ireq13, 0x08, 0x8E);
    Idt::createGate(46, (uint32_t)_ireq14, 0x08, 0x8E);
    Idt::createGate(47, (uint32_t)_ireq15, 0x08, 0x8E);
    Idt::createGate(48, (uint32_t)_ireq16, 0x08, 0x8E);
    Idt::createGate(49, (uint32_t)_ireq17, 0x08, 0x8E);
    Idt::createGate(50, (uint32_t)_ireq18, 0x08, 0x8E);
    Idt::createGate(51, (uint32_t)_ireq19, 0x08, 0x8E);
    Idt::createGate(52, (uint32_t)_ireq20, 0x08, 0x8E);
    Idt::createGate(53, (uint32_t)_ireq21, 0x08, 0x8E);
    Idt::createGate(54, (uint32_t)_ireq22, 0x08, 0x8E);
    Idt::createGate(55, (uint32_t)_ireq23, 0x08, 0x8E);
    Idt::createGate(56, (uint32_t)_ireq24, 0x08, 0x8E);
    Idt::createGate(57, (uint32_t)_ireq25, 0x08, 0x8E);
    Idt::createGate(58, (uint32_t)_ireq26, 0x08, 0x8E);
    Idt::createGate(59, (uint32_t)_ireq27, 0x08, 0x8E);
    Idt::createGate(60, (uint32_t)_ireq28, 0x08, 0x8E);
    Idt::createGate(61, (uint32_t)_ireq29, 0x08, 0x8E);
    Idt::createGate(62, (uint32_t)_ireq30, 0x08, 0x8E);
    Idt::createGate(63, (uint32_t)_ireq31, 0x08, 0x8E);
    Idt::createGate(64, (uint32_t)_ireq32, 0x08, 0x8E);
    Idt::createGate(65, (uint32_t)_ireq33, 0x08, 0x8E);
    Idt::createGate(66, (uint32_t)_ireq34, 0x08, 0x8E);
    Idt::createGate(67, (uint32_t)_ireq35, 0x08, 0x8E);
    Idt::createGate(68, (uint32_t)_ireq36, 0x08, 0x8E);
    Idt::createGate(69, (uint32_t)_ireq37, 0x08, 0x8E);
    Idt::createGate(70, (uint32_t)_ireq38, 0x08, 0x8E);
    Idt::createGate(71, (uint32_t)_ireq39, 0x08, 0x8E);
    Idt::createGate(72, (uint32_t)_ireq40, 0x08, 0x8E);
    Idt::createGate(73, (uint32_t)_ireq41, 0x08, 0x8E);
    Idt::createGate(74, (uint32_t)_ireq42, 0x08, 0x8E);
    Idt::createGate(75, (uint32_t)_ireq43, 0x08, 0x8E);
    Idt::createGate(76, (uint32_t)_ireq44, 0x08, 0x8E);
    Idt::createGate(77, (uint32_t)_ireq45, 0x08, 0x8E);
    Idt::createGate(78, (uint32_t)_ireq46, 0x08, 0x8E);
    Idt::createGate(79, (uint32_t)_ireq47, 0x08, 0x8E);
    Idt::createGate(80, (uint32_t)_ireq48, 0x08, 0x8E);
    Idt::createGate(81, (uint32_t)_ireq49, 0x08, 0x8E);
    Idt::createGate(82, (uint32_t)_ireq50, 0x08, 0x8E);
    Idt::createGate(83, (uint32_t)_ireq51, 0x08, 0x8E);
    Idt::createGate(84, (uint32_t)_ireq52, 0x08, 0x8E);
    Idt::createGate(85, (uint32_t)_ireq53, 0x08, 0x8E);
    Idt::createGate(86, (uint32_t)_ireq54, 0x08, 0x8E);
    Idt::createGate(87, (uint32_t)_ireq55, 0x08, 0x8E);
    Idt::createGate(88, (uint32_t)_ireq56, 0x08, 0x8E);
    Idt::createGate(89, (uint32_t)_ireq57, 0x08, 0x8E);
    Idt::createGate(90, (uint32_t)_ireq58, 0x08, 0x8E);
    Idt::createGate(91, (uint32_t)_ireq59, 0x08, 0x8E);
    Idt::createGate(92, (uint32_t)_ireq60, 0x08, 0x8E);
    Idt::createGate(93, (uint32_t)_ireq61, 0x08, 0x8E);
    Idt::createGate(94, (uint32_t)_ireq62, 0x08, 0x8E);
    Idt::createGate(95, (uint32_t)_ireq63, 0x08, 0x8E);
    Idt::createGate(96, (uint32_t)_ireq64, 0x08, 0x8E);
    Idt::createGate(97, (uint32_t)_ireq65, 0x08, 0x8E);
    Idt::createGate(98, (uint32_t)_ireq66, 0x08, 0x8E);
    Idt::createGate(99, (uint32_t)_ireq67, 0x08, 0x8E);
    Idt::createGate(100, (uint32_t)_ireq68, 0x08, 0x8E);
    Idt::createGate(101, (uint32_t)_ireq69, 0x08, 0x8E);
    Idt::createGate(102, (uint32_t)_ireq70, 0x08, 0x8E);
    Idt::createGate(103, (uint32_t)_ireq71, 0x08, 0x8E);
    Idt::createGate(104, (uint32_t)_ireq72, 0x08, 0x8E);
    Idt::createGate(105, (uint32_t)_ireq73, 0x08, 0x8E);
    Idt::createGate(106, (uint32_t)_ireq74, 0x08, 0x8E);
    Idt::createGate(107, (uint32_t)_ireq75, 0x08, 0x8E);
    Idt::createGate(108, (uint32_t)_ireq76, 0x08, 0x8E);
    Idt::createGate(109, (uint32_t)_ireq77, 0x08, 0x8E);
    Idt::createGate(110, (uint32_t)_ireq78, 0x08, 0x8E);
    Idt::createGate(111, (uint32_t)_ireq79, 0x08, 0x8E);
    Idt::createGate(112, (uint32_t)_ireq80, 0x08, 0x8E);
    Idt::createGate(113, (uint32_t)_ireq81, 0x08, 0x8E);
    Idt::createGate(114, (uint32_t)_ireq82, 0x08, 0x8E);
    Idt::createGate(115, (uint32_t)_ireq83, 0x08, 0x8E);
    Idt::createGate(116, (uint32_t)_ireq84, 0x08, 0x8E);
    Idt::createGate(117, (uint32_t)_ireq85, 0x08, 0x8E);
    Idt::createGate(118, (uint32_t)_ireq86, 0x08, 0x8E);
    Idt::createGate(119, (uint32_t)_ireq87, 0x08, 0x8E);
    Idt::createGate(120, (uint32_t)_ireq88, 0x08, 0x8E);
    Idt::createGate(121, (uint32_t)_ireq89, 0x08, 0x8E);
    Idt::createGate(122, (uint32_t)_ireq90, 0x08, 0x8E);
    Idt::createGate(123, (uint32_t)_ireq91, 0x08, 0x8E);
    Idt::createGate(124, (uint32_t)_ireq92, 0x08, 0x8E);
    Idt::createGate(125, (uint32_t)_ireq93, 0x08, 0x8E);
    Idt::createGate(126, (uint32_t)_ireq94, 0x08, 0x8E);
    Idt::createGate(127, (uint32_t)_ireq95, 0x08, 0x8E);
    Idt::createGate(128, (uint32_t)_ireqSyscall, 0x08, 0xEE);
    Idt::createGate(129, (uint32_t)_ireq97, 0x08, 0x8E);
    Idt::createGate(130, (uint32_t)_ireq98, 0x08, 0x8E);
    Idt::createGate(131, (uint32_t)_ireq99, 0x08, 0x8E);
    Idt::createGate(132, (uint32_t)_ireq100, 0x08, 0x8E);
    Idt::createGate(133, (uint32_t)_ireq101, 0x08, 0x8E);
    Idt::createGate(134, (uint32_t)_ireq102, 0x08, 0x8E);
    Idt::createGate(135, (uint32_t)_ireq103, 0x08, 0x8E);
    Idt::createGate(136, (uint32_t)_ireq104, 0x08, 0x8E);
    Idt::createGate(137, (uint32_t)_ireq105, 0x08, 0x8E);
    Idt::createGate(138, (uint32_t)_ireq106, 0x08, 0x8E);
    Idt::createGate(139, (uint32_t)_ireq107, 0x08, 0x8E);
    Idt::createGate(140, (uint32_t)_ireq108, 0x08, 0x8E);
    Idt::createGate(141, (uint32_t)_ireq109, 0x08, 0x8E);
    Idt::createGate(142, (uint32_t)_ireq110, 0x08, 0x8E);
    Idt::createGate(143, (uint32_t)_ireq111, 0x08, 0x8E);
    Idt::createGate(144, (uint32_t)_ireq112, 0x08, 0x8E);
    Idt::createGate(145, (uint32_t)_ireq113, 0x08, 0x8E);
    Idt::createGate(146, (uint32_t)_ireq114, 0x08, 0x8E);
    Idt::createGate(147, (uint32_t)_ireq115, 0x08, 0x8E);
    Idt::createGate(148, (uint32_t)_ireq116, 0x08, 0x8E);
    Idt::createGate(149, (uint32_t)_ireq117, 0x08, 0x8E);
    Idt::createGate(150, (uint32_t)_ireq118, 0x08, 0x8E);
    Idt::createGate(151, (uint32_t)_ireq119, 0x08, 0x8E);
    Idt::createGate(152, (uint32_t)_ireq120, 0x08, 0x8E);
    Idt::createGate(153, (uint32_t)_ireq121, 0x08, 0x8E);
    Idt::createGate(154, (uint32_t)_ireq122, 0x08, 0x8E);
    Idt::createGate(155, (uint32_t)_ireq123, 0x08, 0x8E);
    Idt::createGate(156, (uint32_t)_ireq124, 0x08, 0x8E);
    Idt::createGate(157, (uint32_t)_ireq125, 0x08, 0x8E);
    Idt::createGate(158, (uint32_t)_ireq126, 0x08, 0x8E);
    Idt::createGate(159, (uint32_t)_ireq127, 0x08, 0x8E);
    Idt::createGate(160, (uint32_t)_ireq128, 0x08, 0x8E);
    Idt::createGate(161, (uint32_t)_ireq129, 0x08, 0x8E);
    Idt::createGate(162, (uint32_t)_ireq130, 0x08, 0x8E);
    Idt::createGate(163, (uint32_t)_ireq131, 0x08, 0x8E);
    Idt::createGate(164, (uint32_t)_ireq132, 0x08, 0x8E);
    Idt::createGate(165, (uint32_t)_ireq133, 0x08, 0x8E);
    Idt::createGate(166, (uint32_t)_ireq134, 0x08, 0x8E);
    Idt::createGate(167, (uint32_t)_ireq135, 0x08, 0x8E);
    Idt::createGate(168, (uint32_t)_ireq136, 0x08, 0x8E);
    Idt::createGate(169, (uint32_t)_ireq137, 0x08, 0x8E);
    Idt::createGate(170, (uint32_t)_ireq138, 0x08, 0x8E);
    Idt::createGate(171, (uint32_t)_ireq139, 0x08, 0x8E);
    Idt::createGate(172, (uint32_t)_ireq140, 0x08, 0x8E);
    Idt::createGate(173, (uint32_t)_ireq141, 0x08, 0x8E);
    Idt::createGate(174, (uint32_t)_ireq142, 0x08, 0x8E);
    Idt::createGate(175, (uint32_t)_ireq143, 0x08, 0x8E);
    Idt::createGate(176, (uint32_t)_ireq144, 0x08, 0x8E);
    Idt::createGate(177, (uint32_t)_ireq145, 0x08, 0x8E);
    Idt::createGate(178, (uint32_t)_ireq146, 0x08, 0x8E);
    Idt::createGate(179, (uint32_t)_ireq147, 0x08, 0x8E);
    Idt::createGate(180, (uint32_t)_ireq148, 0x08, 0x8E);
    Idt::createGate(181, (uint32_t)_ireq149, 0x08, 0x8E);
    Idt::createGate(182, (uint32_t)_ireq150, 0x08, 0x8E);
    Idt::createGate(183, (uint32_t)_ireq151, 0x08, 0x8E);
    Idt::createGate(184, (uint32_t)_ireq152, 0x08, 0x8E);
    Idt::createGate(185, (uint32_t)_ireq153, 0x08, 0x8E);
    Idt::createGate(186, (uint32_t)_ireq154, 0x08, 0x8E);
    Idt::createGate(187, (uint32_t)_ireq155, 0x08, 0x8E);
    Idt::createGate(188, (uint32_t)_ireq156, 0x08, 0x8E);
    Idt::createGate(189, (uint32_t)_ireq157, 0x08, 0x8E);
    Idt::createGate(190, (uint32_t)_ireq158, 0x08, 0x8E);
    Idt::createGate(191, (uint32_t)_ireq159, 0x08, 0x8E);
    Idt::createGate(192, (uint32_t)_ireq160, 0x08, 0x8E);
    Idt::createGate(193, (uint32_t)_ireq161, 0x08, 0x8E);
    Idt::createGate(194, (uint32_t)_ireq162, 0x08, 0x8E);
    Idt::createGate(195, (uint32_t)_ireq163, 0x08, 0x8E);
    Idt::createGate(196, (uint32_t)_ireq164, 0x08, 0x8E);
    Idt::createGate(197, (uint32_t)_ireq165, 0x08, 0x8E);
    Idt::createGate(198, (uint32_t)_ireq166, 0x08, 0x8E);
    Idt::createGate(199, (uint32_t)_ireq167, 0x08, 0x8E);
    Idt::createGate(200, (uint32_t)_ireq168, 0x08, 0x8E);
    Idt::createGate(201, (uint32_t)_ireq169, 0x08, 0x8E);
    Idt::createGate(202, (uint32_t)_ireq170, 0x08, 0x8E);
    Idt::createGate(203, (uint32_t)_ireq171, 0x08, 0x8E);
    Idt::createGate(204, (uint32_t)_ireq172, 0x08, 0x8E);
    Idt::createGate(205, (uint32_t)_ireq173, 0x08, 0x8E);
    Idt::createGate(206, (uint32_t)_ireq174, 0x08, 0x8E);
    Idt::createGate(207, (uint32_t)_ireq175, 0x08, 0x8E);
    Idt::createGate(208, (uint32_t)_ireq176, 0x08, 0x8E);
    Idt::createGate(209, (uint32_t)_ireq177, 0x08, 0x8E);
    Idt::createGate(210, (uint32_t)_ireq178, 0x08, 0x8E);
    Idt::createGate(211, (uint32_t)_ireq179, 0x08, 0x8E);
    Idt::createGate(212, (uint32_t)_ireq180, 0x08, 0x8E);
    Idt::createGate(213, (uint32_t)_ireq181, 0x08, 0x8E);
    Idt::createGate(214, (uint32_t)_ireq182, 0x08, 0x8E);
    Idt::createGate(215, (uint32_t)_ireq183, 0x08, 0x8E);
    Idt::createGate(216, (uint32_t)_ireq184, 0x08, 0x8E);
    Idt::createGate(217, (uint32_t)_ireq185, 0x08, 0x8E);
    Idt::createGate(218, (uint32_t)_ireq186, 0x08, 0x8E);
    Idt::createGate(219, (uint32_t)_ireq187, 0x08, 0x8E);
    Idt::createGate(220, (uint32_t)_ireq188, 0x08, 0x8E);
    Idt::createGate(221, (uint32_t)_ireq189, 0x08, 0x8E);
    Idt::createGate(222, (uint32_t)_ireq190, 0x08, 0x8E);
    Idt::createGate(223, (uint32_t)_ireq191, 0x08, 0x8E);
    Idt::createGate(224, (uint32_t)_ireq192, 0x08, 0x8E);
    Idt::createGate(225, (uint32_t)_ireq193, 0x08, 0x8E);
    Idt::createGate(226, (uint32_t)_ireq194, 0x08, 0x8E);
    Idt::createGate(227, (uint32_t)_ireq195, 0x08, 0x8E);
    Idt::createGate(228, (uint32_t)_ireq196, 0x08, 0x8E);
    Idt::createGate(229, (uint32_t)_ireq197, 0x08, 0x8E);
    Idt::createGate(230, (uint32_t)_ireq198, 0x08, 0x8E);
    Idt::createGate(231, (uint32_t)_ireq199, 0x08, 0x8E);
    Idt::createGate(232, (uint32_t)_ireq200, 0x08, 0x8E);
    Idt::createGate(233, (uint32_t)_ireq201, 0x08, 0x8E);
    Idt::createGate(234, (uint32_t)_ireq202, 0x08, 0x8E);
    Idt::createGate(235, (uint32_t)_ireq203, 0x08, 0x8E);
    Idt::createGate(236, (uint32_t)_ireq204, 0x08, 0x8E);
    Idt::createGate(237, (uint32_t)_ireq205, 0x08, 0x8E);
    Idt::createGate(238, (uint32_t)_ireq206, 0x08, 0x8E);
    Idt::createGate(239, (uint32_t)_ireq207, 0x08, 0x8E);
    Idt::createGate(240, (uint32_t)_ireq208, 0x08, 0x8E);
    Idt::createGate(241, (uint32_t)_ireq209, 0x08, 0x8E);
    Idt::createGate(242, (uint32_t)_ireq210, 0x08, 0x8E);
    Idt::createGate(243, (uint32_t)_ireq211, 0x08, 0x8E);
    Idt::createGate(244, (uint32_t)_ireq212, 0x08, 0x8E);
    Idt::createGate(245, (uint32_t)_ireq213, 0x08, 0x8E);
    Idt::createGate(246, (uint32_t)_ireq214, 0x08, 0x8E);
    Idt::createGate(247, (uint32_t)_ireq215, 0x08, 0x8E);
    Idt::createGate(248, (uint32_t)_ireq216, 0x08, 0x8E);
    Idt::createGate(249, (uint32_t)_ireq217, 0x08, 0x8E);
    Idt::createGate(250, (uint32_t)_ireq218, 0x08, 0x8E);
    Idt::createGate(251, (uint32_t)_ireq219, 0x08, 0x8E);
    Idt::createGate(252, (uint32_t)_ireq220, 0x08, 0x8E);
    Idt::createGate(253, (uint32_t)_ireq221, 0x08, 0x8E);
    Idt::createGate(254, (uint32_t)_ireq222, 0x08, 0x8E);
    Idt::createGate(255, (uint32_t)_ireq223, 0x08, 0x8E);
}
