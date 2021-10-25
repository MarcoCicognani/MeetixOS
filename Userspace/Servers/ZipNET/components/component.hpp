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

#ifndef __COMPONENT__
#define __COMPONENT__

#include <components/BoundsEventComponent.hpp>
#include <components/EventListenerInfo.hpp>
#include <events/event.hpp>
#include <Graphics/Context.hh>
#include <Graphics/Metrics/Rectangle.hh>
#include <GUI/Protocol.hh>
#include <layout/LayoutManager.hpp>
#include <map>
#include <stdio.h>
#include <Tasking/Lock.hh>
#include <vector>

using namespace std;

// forward declarations
class Window_t;
class Component_t;

enum class ChildComponentRefType
{
    Default,
    Internal
};

struct ChildComponentRef {
public:
    Component_t*          m_component;
    ChildComponentRefType m_ref_type;
};

struct ListenerEntry {
public:
    UiComponentEventType m_event_type;
    EventListenerInfo_t  m_event_listener_info;
    ListenerEntry*       m_previous;
    ListenerEntry*       m_next;
};

/**
 * A component requirement is a flag that is put on the component
 * to mark that one specific action or multiple actions are required
 * for the component.
 *
 * For example, when a component has changed model data in an update
 * cycle, the component may also want to repaint its content, therefore
 * marks itself as paint-required.
 */
#define COMPONENT_REQUIREMENT_NONE   0
#define COMPONENT_REQUIREMENT_PAINT  1
#define COMPONENT_REQUIREMENT_LAYOUT 2
#define COMPONENT_REQUIREMENT_UPDATE 4
#define COMPONENT_REQUIREMENT_ALL    0xFFFFFFFF

typedef uint32_t ComponentRequirement_t;

/**
 *
 */
class Component_t : public BoundsEventComponent_t {
private:
    Graphics::Metrics::Rectangle bounds{};
    Component_t*                 parent{ nullptr };
    Tasking::Lock                m_children_lock{};
    vector<ChildComponentRef>   children{};

    Graphics::Metrics::Dimension minimumSize;
    Graphics::Metrics::Dimension preferredSize;
    Graphics::Metrics::Dimension maximumSize;

    ComponentRequirement_t requirements;
    ComponentRequirement_t childRequirements;

    ListenerEntry* listeners{ nullptr };

    int zIndex = 1000;

protected:
    LayoutManager_t*  layoutManager;
    Graphics::Context graphics;

    int  type = -1;
    bool visible;

public:
    UiComponentID id;

    /**
     * Creates the component; initially marks it as dirty
     * and sets no parent
     */
    Component_t(bool transparentBackground = false)
        : id(-1)
        , graphics(transparentBackground)
        , visible(true)
        , requirements(COMPONENT_REQUIREMENT_ALL)
        , childRequirements(COMPONENT_REQUIREMENT_ALL)
        , parent(nullptr)
        , layoutManager(nullptr)
        , BoundsEventComponent_t(this) {
    }

    /**
     * Destroys the component
     */
    virtual ~Component_t();

    /**
     *
     */
    void setZIndex(int zIndex) {
        this->zIndex = zIndex;
    }

    /**
     * Returns a Pointer to the components graphics
     */
    Graphics::Context* getGraphics() {
        return &graphics;
    }

    /**
     * Returns the components parent
     *
     * @return the components parent
     */
    Component_t* getParent() {
        return parent;
    }

    /**
     *
     */
    vector<ChildComponentRef>& getChildren() {
        return children;
    }

    /**
     *
     */
    bool isVisible() const {
        return visible;
    }

    /**
     *
     */
    bool canHandleEvents() const;

    /**
     *
     */
    void setVisible(bool visible);

    /**
     *
     */
    virtual void setFocus(bool focus) {
    }

    /**
     * Sets the bounds of the component and recreates its graphics buffer.
     *
     * @param bounds	the new bounds of the component
     */
    void setBounds(const Graphics::Metrics::Rectangle& bounds);

    /**
     * Returns the bounds of the component.
     *
     * @return the bounds
     */
    Graphics::Metrics::Rectangle getBounds() const {
        return bounds;
    }

    /**
     *
     */
    void setPreferredSize(const Graphics::Metrics::Dimension& size);

    /**
     *
     */
    virtual Graphics::Metrics::Dimension getPreferredSize() {
        return preferredSize;
    }

    /**
     *
     */
    void setMinimumSize(const Graphics::Metrics::Dimension& size);

    /**
     *
     */
    Graphics::Metrics::Dimension getMinimumSize() const {
        return minimumSize;
    }

    /**
     *
     */
    void setMaximumSize(const Graphics::Metrics::Dimension& size);

    /**
     *
     */
    Graphics::Metrics::Dimension getMaximumSize() const {
        return maximumSize;
    }

    /**
     * This method is used to blit the component and all of its children
     * to the out buffer
     *
     * @param absClip	absolute bounds that may not be exceeded
     * @param position	absolute screen position to blit to
     */
    void blit(Graphics::Context*           out,
              Graphics::Metrics::Rectangle absClip,
              Graphics::Metrics::Point     position);

    /**
     * Adds the given component as a child to this component
     *
     * @param comp	the component to add
     */
    virtual void addChild(Component_t*          comp,
                          ChildComponentRefType ref_type = ChildComponentRefType::Default);

    /**
     * Removes the given component from this component
     *
     * @param comp	the component to remove
     */
    void removeChild(Component_t* comp);

    /**
     * Returns the component at the given Point
     *
     * @param p		the Point
     */
    virtual Component_t* getComponentAt(Graphics::Metrics::Point p);

    /**
     * Returns the first in the hierarchy that is a Window
     *
     * @return the window
     */
    virtual Window_t* getWindow();

    virtual bool isWindow() const {
        return false;
    }

    /**
     * Brings this component to the front
     */
    virtual void bringToFront();

    /**
     * Brings the given child component to the front
     *
     * @param comp	the child component
     */
    virtual void bringChildToFront(Component_t* comp);

    /**
     * Returns the location of the component on screen
     *
     * @return the location
     */
    virtual Graphics::Metrics::Point getLocationOnScreen();

    /**
     * Handles the given event
     *
     * @return true if it was handled, otherwise false
     */
    virtual bool handle(Event_t& event);

    /**
     *
     */
    virtual void
    handleBoundChange(Graphics::Metrics::Rectangle oldBounds) { /* May be implemented by subtypes */
    }

    /**
     *
     */
    void setLayoutManager(LayoutManager_t* layoutManager);

    /**
     *
     */
    LayoutManager_t* getLayoutManager() const {
        return layoutManager;
    }

    /**
     * Marks the given area as dirty so it is copied to the framebuffer
     *
     * @param rect	the Rectangle to mark dirty
     */
    virtual void markDirty(Graphics::Metrics::Rectangle rect);

    /**
     * Marks the entire component as dirty
     */
    virtual void markDirty() {
        markDirty(Graphics::Metrics::Rectangle(0, 0, bounds.width(), bounds.height()));
    }

    bool getChildReference(Component_t* child, ChildComponentRef& out);

    /**
     * Places the flag for the given requirement on the parent component (if non-null).
     */
    void markParentFor(ComponentRequirement_t req);

    /**
     * Places the flag for the given requirement on this component.
     */
    void markFor(ComponentRequirement_t req);

    /**
     * Places the flag for the given requirement in the list of child-requirements.
     */
    void mark_children_for(ComponentRequirement_t req);

    /**
     * Resolves the given requirement
     */
    void resolveRequirement(ComponentRequirement_t req);

    /**
     * This method is called by the window manager if the layout requirement flag is set.
     * The component may here change the bounds of each child component and also change its
     * own preferred size.
     */
    virtual void layout();

    /**
     * This method is called by the window manager if the update requirement flag is set.
     * The component may here change the contents of it's model.
     */
    virtual void update();

    /**
     * This method is called by the window manager if the paint requirement flag is set.
     * The component may here repaint itself.
     */
    virtual void paint();

    /**
     *
     */
    virtual bool getNumericProperty(int property, uint32_t* out);

    /**
     *
     */
    virtual bool setNumericProperty(int property, uint32_t value);

    /**
     *
     */
    void setListener(UiComponentEventType eventType, Tid targetThread, UiComponentID id);

    /**
     *
     */
    bool getListener(UiComponentEventType eventType, EventListenerInfo_t& out);

    /**
     *
     */
    void clearSurface();

    /**
     *
     */
    bool isChildOf(Component_t* c);

    /**
     *
     */
    inline int getComponentType() {
        return type;
    }
};

#endif
