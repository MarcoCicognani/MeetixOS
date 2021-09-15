/*********************************************************************************
 * MeetiX OS By MeetiX OS Project [Marco Cicognani]                               *
 * 																			     *
 * This program is free software; you can redistribute it and/or                  *
 * modify it under the terms of the GNU General Public License                    *
 * as published by the Free Software Foundation; either version 2				 *
 * of the License, or (char *argumentat your option) any later version.			 *
 *																				 *
 * This program is distributed in the hope that it will be useful,				 *
 * but WITHout ANY WARRANTY; without even the implied warranty of                 *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 				 *
 * GNU General Public License for more details.
 **
 *																				 *
 * You should have received a copy of the GNU General Public License				 *
 * along with this program; if not, write to the Free Software                    *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
 **********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_COMPONENT
#define MEETIX_LIBRARY_UI_COMPONENT

#include <cstdint>
#include <eva.h>
#include <eva/utils/local.hpp>
#include <graphics/metrics/rectangle.hpp>
#include <gui/boundseventcomponent.hpp>
#include <gui/componentregistry.hpp>
#include <gui/mouselistener.hpp>
#include <gui/properties.hpp>
#include <gui/ui.hpp>
#include <gui/uispech.hpp>
#include <map>
#include <stdio.h>
#include <utils/valplacer.hpp>

/**
 *
 */
class Component : public BoundsEventComponent {
protected:
    UiComponentID                             id;
    UiComponentType                           type;
    std::map<UiComponentEventType, Listener*> listeners;

    /**
     *
     */
    Component(UiComponentID id, UiComponentType tp)
        : id(id), type(tp), BoundsEventComponent(this, id) {
    }

    /**
     *
     */
    template<typename ComponentType>
    struct Concrete : ComponentType {
        Concrete(UiComponentID id) : ComponentType(id) {
        }
    };

    /**
     *
     */
    template<typename ComponentType, UiComponentType ComponentConstant>
    static ComponentType* createComponent() {
        if ( !UiInitialized )
            return 0;

        // send initialization request
        MessageTransaction tx = GetMessageTxId();

        UiCreateComponentRequest request;
        request.header.id = UI_PROTOCOL_CREATE_COMPONENT;
        request.type      = ComponentConstant;
        SendMessageT(UiDelegateTid, &request, sizeof(UiCreateComponentRequest), tx);

        // read response
        size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiCreateComponentResponse);
        Local<uint8_t> buffer(new uint8_t[bufferSize]);

        if ( ReceiveMessageT(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            UiCreateComponentResponse* response
                = (UiCreateComponentResponse*)MESSAGE_CONTENT(buffer());

            // create the component
            UiComponentID id = response->id;
            if ( response->status == UI_PROTOCOL_SUCCESS ) {
                Concrete<ComponentType>* component = new Concrete<ComponentType>(id);
                ComponentRegistry::instance().add(component);

                return component;
            }
        }

        return 0;
    }

private:
    void removeFromZipNetRegex();

public:
    /**
     *
     */
    virtual ~Component();

    /**
     *
     */
    bool addChild(Component* c);

    /**
     *
     */
    bool setBounds(Rectangle rect);

    /**
     *
     */
    Rectangle getBounds();

    /**
     *
     */
    bool setVisible(bool visible);

    /*
     *
     */
    bool setFocus(bool focus);

    /**
     *
     */
    bool setNumericProperty(int property, uint32_t value);

    /**
     *
     */
    bool getNumericProperty(int property, uint32_t* out);

    /**
     *
     */
    bool setListener(UiComponentEventType eventType, Listener* listener);

    /**
     *
     */
    bool setMouseListener(MouseListener* listener);

    /**
     *
     */
    void handle(UiComponentEventHeader* header);

    /**
     *
     */
    bool setLayout(UiLayoutManager layout);

    /**
     *
     */
    inline UiComponentID getID() {
        return id;
    }
};

#endif
