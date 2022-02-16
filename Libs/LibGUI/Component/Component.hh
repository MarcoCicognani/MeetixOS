/**
 * @brief
 * This file is part of the MeetiX Operating System.
 * Copyright (c) 2017-2022, Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @developers
 * Marco Cicognani (marco.cicognani@meetixos.org)
 *
 * @license
 * GNU General Public License version 3
 */

#pragma once

#include <Api.h>
#include <Api/utils/local.hpp>
#include <cstdint>
#include <cstdio>
#include <LibGraphics/Metrics/Rectangle.hh>
#include <LibGUI/Application.hh>
#include <LibGUI/Component/BoundsEventComponent.hh>
#include <LibGUI/Component/ComponentRegistry.hh>
#include <LibGUI/Listener/MouseListener.hpp>
#include <LibGUI/Properties.hh>
#include <LibGUI/Protocol.hh>
#include <map>

/**
 *
 */
class Component : public BoundsEventComponent {
protected:
    UiComponentID                             m_component_id;
    UiComponentType                           type;
    std::map<UiComponentEventType, Listener*> listeners;

    /**
     *
     */
    Component(UiComponentID id, UiComponentType tp)
        : m_component_id(id)
        , type(tp)
        , BoundsEventComponent(this, id) {}

    /**
     *
     */
    template<typename ComponentType>
    struct Concrete : ComponentType {
        Concrete(UiComponentID id)
            : ComponentType(id) {}
    };

    /**
     *
     */
    template<typename ComponentType, UiComponentType ComponentConstant>
    static ComponentType* create() {
        if ( !UiInitialized )
            return 0;

        // send initialization request
        MessageTransaction tx = s_get_message_tx_id();

        UiCreateComponentRequest request;
        request.header.m_command = UI_PROTOCOL_CREATE_COMPONENT;
        request.type             = ComponentConstant;
        s_send_message_t(UiDelegateTid, &request, sizeof(UiCreateComponentRequest), tx);

        // read response
        size_t         bufferSize = sizeof(MessageHeader) + sizeof(UiCreateComponentResponse);
        Local<uint8_t> buffer(new uint8_t[bufferSize]);

        if ( s_receive_message_t(buffer(), bufferSize, tx) == MESSAGE_RECEIVE_STATUS_SUCCESSFUL ) {
            UiCreateComponentResponse* response = (UiCreateComponentResponse*)MESSAGE_CONTENT(buffer());

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
    void remove_from_wm_registry();

public:
    /**
     *
     */
    ~Component() override;

    /**
     *
     */
    bool add_child(Component* c);

    /**
     *
     */
    bool set_bounds(Graphics::Metrics::Rectangle rect);

    /**
     *
     */
    Graphics::Metrics::Rectangle bounds();

    /**
     *
     */
    bool set_visible(bool visible);

    /*
     *
     */
    bool set_focus(bool focus);

    /**
     *
     */
    bool set_numeric_property(int property, uint32_t value);

    /**
     *
     */
    bool get_numeric_property(int property, uint32_t* out);

    /**
     *
     */
    bool set_listener(UiComponentEventType eventType, Listener* listener);

    /**
     *
     */
    bool set_mouse_listener(MouseListener* listener);

    /**
     *
     */
    void handle(UiComponentEventHeader* header);

    /**
     *
     */
    bool set_layout(UiLayoutManager layout);

    /**
     *
     */
    [[nodiscard]] inline UiComponentID id() const { return this->m_component_id; }
};
