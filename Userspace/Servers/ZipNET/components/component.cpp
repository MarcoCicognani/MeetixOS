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

#include <algorithm>
#include <cairo/cairo.h>
#include <components/component.hpp>
#include <components/window.hpp>
#include <events/KeyEvent.hpp>
#include <events/locatable.hpp>
#include <events/MouseEvent.hpp>
#include <gui/properties.hpp>
#include <layout/FlowLayoutManager.hpp>
#include <layout/GridLayoutManager.hpp>

/**
 *
 */
Component_t::~Component_t() {
    if ( layoutManager )
        delete layoutManager;
}

/**
 *
 */
void Component_t::setBounds(const Rectangle& newBounds) {
    Rectangle oldBounds = bounds;

    markDirty();
    bounds = newBounds;
    if ( bounds.width < minimumSize.width )
        bounds.width = minimumSize.width;

    if ( bounds.height < minimumSize.height )
        bounds.height = minimumSize.height;
    markDirty();

    if ( oldBounds.width != bounds.width || oldBounds.height != bounds.height ) {
        graphics.resize(bounds.width, bounds.height);
        markFor(COMPONENT_REQUIREMENT_LAYOUT);
        markFor(COMPONENT_REQUIREMENT_UPDATE);
        markFor(COMPONENT_REQUIREMENT_PAINT);

        handleBoundChange(oldBounds);
    }

    fireBoundsChange(bounds);
}

/**
 *
 */
bool Component_t::canHandleEvents() const {
    if ( !visible )
        return false;

    if ( parent )
        return parent->canHandleEvents();

    return true;
}

/**
 *
 */
void Component_t::setVisible(bool visible) {
    this->visible = visible;

    markDirty();
    markFor(COMPONENT_REQUIREMENT_ALL);
}

/**
 *
 */
void Component_t::markDirty(Rectangle rect) {
    if ( parent ) {
        rect.x = rect.x + bounds.x;
        rect.y = rect.y + bounds.y;
        parent->markDirty(rect);
    }
}

/**
 *
 */
void Component_t::blit(Graphics* out, Rectangle absClip, Point position) {
    if ( this->visible ) {
        if ( graphics.getContext() != 0 )
            graphics.blitTo(out, absClip, position);

        Rectangle ownAbsBounds = getBounds();
        ownAbsBounds.x         = position.x;
        ownAbsBounds.y         = position.y;
        int newTop
            = absClip.getTop() > ownAbsBounds.getTop() ? absClip.getTop() : ownAbsBounds.getTop();
        int newBottom = absClip.getBottom() < ownAbsBounds.getBottom() ? absClip.getBottom()
                                                                       : ownAbsBounds.getBottom();
        int newLeft   = absClip.getLeft() > ownAbsBounds.getLeft() ? absClip.getLeft()
                                                                   : ownAbsBounds.getLeft();
        int newRight  = absClip.getRight() < ownAbsBounds.getRight() ? absClip.getRight()
                                                                     : ownAbsBounds.getRight();

        Rectangle thisClip = Rectangle(newLeft, newTop, newRight - newLeft, newBottom - newTop);
        for ( Component_t* c : children )
            if ( c->visible )
                c->blit(out, thisClip, Point(position.x + c->bounds.x, position.y + c->bounds.y));
    }
}

/**
 *
 */
void Component_t::addChild(Component_t* comp) {
    if ( comp->parent )
        comp->parent->removeChild(comp);

    children.push_back(comp);

    sort(children.begin(), children.end(), [](Component_t*& cmp1, Component_t*& cmp2) {
        return cmp1->zIndex < cmp2->zIndex;
    });

    comp->parent = this;
    markFor(COMPONENT_REQUIREMENT_LAYOUT);
}

/**
 *
 */
void Component_t::removeChild(Component_t* comp) {
    children.erase(remove(children.begin(), children.end(), comp), children.end());
    comp->parent = 0;
    markFor(COMPONENT_REQUIREMENT_LAYOUT);
}

/**
 *
 */
Component_t* Component_t::getComponentAt(Point p) {
    for ( int i = children.size() - 1; i >= 0; i-- ) {
        Component_t* child = children[i];

        if ( child->bounds.contains(p) )
            return child->getComponentAt(Point(p.x - child->bounds.x, p.y - child->bounds.y));
    }

    return this;
}

/**
 *
 */
Window_t* Component_t::getWindow() {
    Window_t* thisAsWindow = dynamic_cast<Window_t*>(this);
    if ( thisAsWindow != 0 )
        return thisAsWindow;

    if ( parent )
        return parent->getWindow();

    return 0;
}

/**
 *
 */
void Component_t::bringChildToFront(Component_t* comp) {
    for ( uint32_t index = 0; index < children.size(); index++ ) {
        if ( children[index] == comp ) {
            children.erase(children.begin() + index);
            children.push_back(comp);
            markDirty(comp->bounds);
            break;
        }
    }
}

/**
 *
 */
void Component_t::bringToFront() {
    Component_t* parent = getParent();
    if ( parent )
        parent->bringChildToFront(this);
}

/**
 *
 */
Point Component_t::getLocationOnScreen() {
    Point locationOnScreen(bounds.x, bounds.y);

    if ( parent ) {
        Point parentLocationOnScreen = parent->getLocationOnScreen();
        locationOnScreen.x           = locationOnScreen.x + parentLocationOnScreen.x;
        locationOnScreen.y           = locationOnScreen.y + parentLocationOnScreen.y;
    }

    return locationOnScreen;
}

/**
 *
 */
bool Component_t::handle(Event_t& event) {
    Locatable_t* locatable = dynamic_cast<Locatable_t*>(&event);

    for ( int i = children.size() - 1; i >= 0; i-- ) {
        Component_t* child = children[i];

        if ( child->visible ) {
            if ( locatable ) {
                if ( child->bounds.contains(locatable->position) ) {
                    locatable->position.x -= child->bounds.x;
                    locatable->position.y -= child->bounds.y;

                    if ( child->handle(event) )
                        return true;

                    locatable->position.x += child->bounds.x;
                    locatable->position.y += child->bounds.y;
                }
            }

            else if ( child->handle(event) )
                return true;
        }
    }

    // post key event to client
    KeyEvent_t* keyEvent = dynamic_cast<KeyEvent_t*>(&event);
    if ( keyEvent ) {
        EventListenerInfo_t info;
        if ( getListener(UI_COMPONENT_EVENT_TYPE_KEY, info) ) {
            UiComponentKeyEvent postedKeyEvent;
            postedKeyEvent.header.type        = UI_COMPONENT_EVENT_TYPE_KEY;
            postedKeyEvent.header.componentID = info.componentID;
            postedKeyEvent.keyInfo            = keyEvent->info;
            SendMessage(info.targetThread, &postedKeyEvent, sizeof(UiComponentKeyEvent));
        }
    }

    // post mouse event to client
    MouseEvent_t* mouseEvent = dynamic_cast<MouseEvent_t*>(&event);
    if ( mouseEvent ) {
        EventListenerInfo_t info;
        if ( getListener(UI_COMPONENT_EVENT_TYPE_MOUSE, info) ) {
            UiComponentMouseEvent postedEvent;
            postedEvent.header.type        = UI_COMPONENT_EVENT_TYPE_MOUSE;
            postedEvent.header.componentID = info.componentID;
            postedEvent.position           = mouseEvent->position;
            postedEvent.type               = mouseEvent->type;
            postedEvent.buttons            = mouseEvent->buttons;
            SendMessage(info.targetThread, &postedEvent, sizeof(UiComponentMouseEvent));
        }
    }

    return false;
}

/**
 *
 */
void Component_t::setPreferredSize(const Dimension& size) {
    preferredSize = size;
}

/**
 *
 */
void Component_t::setMinimumSize(const Dimension& size) {
    minimumSize = size;
}

/**
 *
 */
void Component_t::setMaximumSize(const Dimension& size) {
    maximumSize = size;
}

/**
 *
 */
void Component_t::setLayoutManager(LayoutManager_t* newMgr) {
    newMgr->setComponent(this);
    this->layoutManager = newMgr;
    markFor(COMPONENT_REQUIREMENT_LAYOUT);
}

/**
 *
 */
void Component_t::layout() {
    if ( layoutManager ) {
        layoutManager->layout();
    }

    markFor(COMPONENT_REQUIREMENT_UPDATE);
}

/**
 *
 */
void Component_t::update() {
    markFor(COMPONENT_REQUIREMENT_PAINT);
}

/**
 *
 */
void Component_t::paint() {
}

/**
 *
 */
void Component_t::markParentFor(ComponentRequirement_t req) {
    if ( parent )
        parent->markFor(req);
}

/**
 *
 */
void Component_t::markFor(ComponentRequirement_t req) {
    requirements |= req;

    if ( parent ) {
        parent->mark_children_for(req);
    }
}

/**
 *
 */
void Component_t::mark_children_for(ComponentRequirement_t req) {
    childRequirements |= req;

    if ( parent ) {
        parent->mark_children_for(req);
    }
}

/**
 *
 */
void Component_t::resolveRequirement(ComponentRequirement_t req) {
    if ( childRequirements & req ) {
        for ( auto child : children ) {
            if ( child->visible ) {
                child->resolveRequirement(req);
            }
        }
        childRequirements &= ~COMPONENT_REQUIREMENT_NONE;
    }

    if ( requirements & req ) {
        if ( req == COMPONENT_REQUIREMENT_LAYOUT ) {
            layout();
        } else if ( req == COMPONENT_REQUIREMENT_UPDATE ) {
            update();
        } else if ( req == COMPONENT_REQUIREMENT_PAINT ) {
            paint();
            markDirty();
        }

        requirements &= ~req;
    }
}

/**
 *
 */
void Component_t::setListener(UiComponentEventType eventType, Tid targetThread, UiComponentID id) {
    EventListenerInfo_t info;
    info.targetThread    = targetThread;
    info.componentID     = id;
    listeners[eventType] = info;
}

/**
 *
 */
bool Component_t::getListener(UiComponentEventType eventType, EventListenerInfo_t& out) {
    if ( listeners.count(eventType) ) {
        out = listeners[eventType];
        return true;
    }
    return false;
}

/**
 *
 */
void Component_t::clearSurface() {
    // clear surface
    cairo_t* cr = graphics.getContext();
    cairo_save(cr);
    cairo_set_source_rgba(cr, 0, 0, 0, 0);
    cairo_set_operator(cr, CAIRO_OPERATOR_SOURCE);
    cairo_paint(cr);
    cairo_restore(cr);
}

/**
 *
 */
bool Component_t::isChildOf(Component_t* c) {
    Component_t* next = parent;
    do {
        if ( next == c )
            return true;
        next = next->getParent();
    } while ( next );

    return false;
}

/**
 *
 */
bool Component_t::getNumericProperty(int property, uint32_t* out) {
    return false;
}

/**
 *
 */
bool Component_t::setNumericProperty(int property, uint32_t value) {
    if ( property == UI_PROPERTY_LAYOUT_MANAGER ) {
        if ( value == UI_LAYOUT_MANAGER_FLOW ) {
            setLayoutManager(new FlowLayoutManager_t());
            return true;
        }

        else if ( value == UI_LAYOUT_MANAGER_GRID ) {
            setLayoutManager(new GridLayoutManager_t(1, 1));
            return true;
        }
    }

    return false;
}
