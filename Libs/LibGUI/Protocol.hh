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

#include <Api.h>
#include <climits>
#include <LibGraphics/Color.hh>
#include <LibGraphics/Metrics/Dimension.hh>
#include <LibGraphics/Metrics/Rectangle.hh>
#include <LibGraphics/Text/Alignment.hh>
#include <LibIO/Keyboard.hh>
#include <string>
#include <vector>

// namespace GUI::Protocol {

static constexpr auto C_REGISTRATION_THREAD_NAME = "ZipNET::registration";
static constexpr auto C_MESSAGE_LEN_MAX          = 4096;
static constexpr auto C_TITLE_LEN_MAX            = 1024;

/*
 * ID for a UI component
 */
using UiComponentID = isize;
using UiListenerID  = isize;

/*
 * A protocol message always starts with the header, the message m_command
 */
enum UiProtocolCommandID {
    UI_PROTOCOL_INITIALIZATION,
    UI_PROTOCOL_CREATE_COMPONENT,
    UI_PROTOCOL_REMOVE_COMPONENT,
    UI_PROTOCOL_REMOVE_COMPONENT_MAP,
    UI_PROTOCOL_ADD_COMPONENT,
    UI_PROTOCOL_SET_TITLE,
    UI_PROTOCOL_SET_GHOST_TITLE,
    UI_PROTOCOL_GET_TITLE,
    UI_PROTOCOL_SET_TITLE_FONT,
    UI_PROTOCOL_SET_BOUNDS,
    UI_PROTOCOL_SET_VISIBLE,
    UI_PROTOCOL_SET_LISTENER,
    UI_PROTOCOL_SET_NUMERIC_PROPERTY,
    UI_PROTOCOL_GET_NUMERIC_PROPERTY,
    UI_PROTOCOL_CANVAS_ACK_BUFFER_REQUEST,
    UI_PROTOCOL_GET_BOUNDS,
    UI_PROTOCOL_CANVAS_BLIT,
    UI_PROTOCOL_REGISTER_DESKTOP_CANVAS,
    UI_PROTOCOL_SET_PNG,
    UI_PROTOCOL_SET_COMPONENT_COLOR,
    UI_PROTOCOL_CHANGE_BACKGROUND,
    UI_PROTOCOL_GET_RESOLUTION,
    UI_PROTOCOL_SET_FOCUS,
    UI_PROTOCOL_SET_FONT_SIZE,
    UI_PROTOCOL_SET_TITLE_ALIGNMENT,
    UI_PROTOCOL_REGISTER_TASK_MANAGER,
    UI_PROTOCOL_SET_MOUSE_CURSOR_FORM,
};

/*
 * Common status for requests
 */
enum UiProtocolStatus {
    UI_PROTOCOL_SUCCESS,
    UI_PROTOCOL_FAIL
};

/*
 * Component types
 */
enum UiComponentType {
    UI_COMPONENT_TYPE_WINDOW,
    UI_COMPONENT_TYPE_BUTTON,
    UI_COMPONENT_TYPE_LABEL,
    UI_COMPONENT_TYPE_TEXTFIELD,
    UI_COMPONENT_TYPE_CANVAS,
    UI_COMPONENT_TYPE_GEOSHAPE,
};

/*
 * Types of events that can be listened to
 */
enum UiComponentEventType {
    UI_COMPONENT_EVENT_TYPE_ACTION,
    UI_COMPONENT_EVENT_TYPE_BOUNDS,
    UI_COMPONENT_EVENT_TYPE_CANVAS_WFA, // "wait for acknowledge"-event
    UI_COMPONENT_EVENT_TYPE_KEY,
    UI_COMPONENT_EVENT_TYPE_FOCUS,
    UI_COMPONENT_EVENT_TYPE_MOUSE,
    UI_COMPONENT_EVENT_TYPE_CLOSE,
};
/*
 *
 */
enum UiLayoutManager {
    UI_LAYOUT_MANAGER_GRID,
    UI_LAYOUT_MANAGER_FLOW
};

/*
 *
 */
struct UiMessageHeader {
    UiProtocolCommandID m_command;
} A_PACKED;

/*
 * Request to initialize interface communications. The window server creates a
 * delegate thread that is responsible for further communications and responds
 * with a <UiInitializeResponse>.
 */
typedef struct {
    UiMessageHeader header;
} A_PACKED UiInitializeRequest;

/*
 * Response for initializing interface communications.
 *
 * @field status
 * 		whether the initialization was successful
 * @field windowServerDelegateThread
 * 		m_command of the thread that is responsible for further window server communication
 */
typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
    Tid              windowServerDelegateThread;
    Tid              windowServerCleanUPThread;
} A_PACKED UiInitializeResponse;

/*
 * Request sent to create a component.
 */
typedef struct {
    UiMessageHeader header;
    UiComponentType type;
} A_PACKED UiCreateComponentRequest;

/*
 * Response when creating a component.
 */
typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
    UiComponentID    id;
} A_PACKED UiCreateComponentResponse;

/*
 * Request to delete a component of current process.
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
} A_PACKED UiRemoveComponentRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiRemoveComponentResponse;

/*
 * Request / response to delete component map of current process.
 */
typedef struct {
    UiMessageHeader header;
} A_PACKED UiRemoveComponentMapRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiRemoveComponentMapResponse;

/*
 * Request/response for adding a child
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   parent;
    UiComponentID   child;
} A_PACKED UiComponentAddChildRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentAddChildResponse;

/*
 * Request/response for setting bounds
 */
typedef struct {
    UiMessageHeader              header;
    UiComponentID                id;
    Graphics::Metrics::Rectangle bounds;
} A_PACKED UiComponentSetBoundsRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetBoundsResponse;

/*
 * Request/response for getting bounds
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
} A_PACKED UiComponentGetBoundsRequest;

typedef struct {
    UiMessageHeader              header;
    UiProtocolStatus             status;
    Graphics::Metrics::Rectangle bounds;
} A_PACKED UiComponentGetBoundsResponse;

/*
 * Request/response for setting components (in)visible
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    bool            visible;
} A_PACKED UiComponentSetVisibleRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetVisibleResponse;

/*
 * Request/response for setting the title on a titled component
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    char            title[C_TITLE_LEN_MAX];
} A_PACKED UiComponentSetTitleRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetTitleResponse;

/*
 *	request/response for setting ghost title on a titled component
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    char            title[C_TITLE_LEN_MAX];
} A_PACKED UiComponentSetGhostTitleRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetGhostTitleResponse;

/*
 * Request/response for getting the title on a titled component
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
} A_PACKED UiComponentGetTitleRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
    char             title[C_TITLE_LEN_MAX];
} A_PACKED UiComponentGetTitleResponse;

/*
 *	Request/response for set font on title
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    char            fontName[PATH_MAX];
} A_PACKED UiComponentSetTitleFontRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetTitleFontResponse;

/*
 * Request/response for getting a numeric property
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    int             property;
} A_PACKED UiComponentGetNumericPropertyRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
    uint32_t         value;
} A_PACKED UiComponentGetNumericPropertyResponse;

/*
 * Request/response for setting a numeric property
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    int             property;
    uint32_t        value;
} A_PACKED UiComponentSetNumericPropertyRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetNumericPropertyResponse;

/*
 * Request/response for acknowledging the buffer update of a m_canvas/blitting the m_canvas
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
} A_PACKED UiComponentCanvasAckBufferRequest;

typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
} A_PACKED UiComponentCanvasBlitRequest;

/*
 * Canvas shared memory header
 */
typedef struct {
    uint16_t paintableWidth;
    uint16_t paintableHeight;
    uint16_t blitX;
    uint16_t blitY;
    uint16_t blitWidth;
    uint16_t blitHeight;
    bool     is_ready;
} A_PACKED UiCanvasSharedMemoryHeader;

#define UI_CANVAS_SHARED_MEMORY_HEADER_SIZE                                                                            \
    ((sizeof(UiCanvasSharedMemoryHeader) - sizeof(UiCanvasSharedMemoryHeader) % sizeof(Address)) + sizeof(Address))

/*
 * Request to register the desktop m_canvas
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   canvasID;
} A_PACKED UiRegisterDesktopCanvasRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiRegisterDesktopCanvasResponse;

/*
 * Event handler registration functions
 */
typedef struct {
    UiMessageHeader      header;
    UiComponentID        id;
    UiComponentEventType eventType;
    Tid                  targetThread;
} A_PACKED UiComponentSetListenerRequest;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetListenerResponse;

/*
 * Event structures
 */
typedef struct {
    UiMessageHeader      header;
    UiComponentEventType type;
    UiComponentID        componentID;
} A_PACKED UiComponentEventHeader;

typedef struct {
    UiComponentEventHeader header;
} A_PACKED UiComponentActionEvent;

typedef struct {
    UiComponentEventHeader       header;
    Graphics::Metrics::Rectangle bounds;
} A_PACKED UiComponentBoundsEvent;

typedef struct {
    UiComponentEventHeader header;
    Address                newBufferAddress;
} A_PACKED UiComponentCanvasWfaEvent;

typedef struct {
    UiComponentEventHeader  header;
    IO::Keyboard::InfoBasic keyInfo;
} A_PACKED UiComponentKeyEvent;

typedef struct {
    UiComponentEventHeader header;
    uint8_t                nowFocused;
} A_PACKED UiComponentFocusEvent;

/*
 *	component png setup/response
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;

    char                     pathToPng[PATH_MAX];
    Graphics::Metrics::Point pngPosition;
} A_PACKED UiComponentSetupPng;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentSetupPngResponse;

/*
 *	Component color setup/response
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;

    Graphics::Color::ArgbGradient shapeColor;
    Graphics::Color::ArgbGradient titleColor;
} A_PACKED UiComponentColor;

typedef struct {
    UiMessageHeader  header;
    UiProtocolStatus status;
} A_PACKED UiComponentColorResponse;

/*
 *	change background
 */
typedef struct {
    UiMessageHeader header;

    char path[PATH_MAX];
} A_PACKED UiChangeBackgroundRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiChangeBackgroundResponse;

/*
 *	send get resolution request
 */
typedef struct {
    UiMessageHeader header;
} A_PACKED UiGetResolution;

/*
 *	get resolution of screen
 */
typedef struct {
    UiProtocolStatus             status;
    Graphics::Metrics::Dimension resolution;
} A_PACKED UiGetResolutionResponse;

/*
 *	close event of window
 */
typedef struct {
    UiComponentEventHeader header;
} A_PACKED UiComponentCloseEvent;

/*
 *	request/response of focus event of component
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    bool            focus;
} A_PACKED UiComponentFocusRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiComponentFocusResponse;

/*
 * request/response to set label font size
 */
typedef struct {
    UiMessageHeader header;
    UiComponentID   id;
    int             size;
} A_PACKED UiSetFontSizeRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiSetFontSizeResponse;

/*
 *	request/reponse to set title alignment
 */
typedef struct {
    UiMessageHeader           header;
    UiComponentID             id;
    Graphics::Text::Alignment alignment;
} A_PACKED UiSetTitleAlignmentRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiSetTitleAlignmentResponse;

/*
 *	request/response to register task manager on component
 */
typedef struct {
    UiMessageHeader              header;
    UiComponentID                id;
    Graphics::Metrics::Rectangle bounds;
} A_PACKED UiRegisterTaskManagerRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiRegisterTaskManagerResponse;

/*
 * request/response to change form of mouse cursor
 */
typedef struct {
    UiMessageHeader header;
    char            name[50];
} A_PACKED UiSetMouseCursorFormRequest;

typedef struct {
    UiProtocolStatus status;
} A_PACKED UiSetMouseCursorFormResponse;

/*
 * Mouse events
 */
using MouseButton                              = u8;
static constexpr MouseButton MOUSE_BUTTON_NONE = 0;
static constexpr MouseButton MOUSE_BUTTON_1    = 1;
static constexpr MouseButton MOUSE_BUTTON_2    = 2;
static constexpr MouseButton MOUSE_BUTTON_3    = 4;

enum MouseEventType {
    MOUSE_EVENT_NONE,
    MOUSE_EVENT_MOVE,
    MOUSE_EVENT_PRESS,
    MOUSE_EVENT_RELEASE,
    MOUSE_EVENT_DRAG_RELEASE,
    MOUSE_EVENT_DRAG,
    MOUSE_EVENT_ENTER,
    MOUSE_EVENT_LEAVE,
};

/*
 *	mouse event data struct
 */
typedef struct {
    UiComponentEventHeader   header;
    Graphics::Metrics::Point position;
    MouseEventType           type;
    MouseButton              buttons;
} A_PACKED UiComponentMouseEvent;

//} /* namespace GUI::Protocol */
