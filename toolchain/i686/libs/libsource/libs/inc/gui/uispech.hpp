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
* GNU General Public License for more details.									 *
*																				 *
* You should have received a copy of the GNU General Public License				 *
* along with this program; if not, write to the Free Software                    *
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA *
**********************************************************************************/

#ifndef MEETIX_LIBRARY_UI_INTERFACE_SPECIFICATION
#define MEETIX_LIBRARY_UI_INTERFACE_SPECIFICATION

#include <string>
#include <eva.h>
#include <graphics/metrics/rectangle.hpp>
#include <graphics/metrics/dimension.hpp>
#include <graphics/color.hpp>
#include <graphics/text/textalign.hpp>
#include <io/keyboard.hpp>
#include <vector>

/*
 * This UI interface specification defines the messages
 * that the active window manager must understand.
 */
#define UI_REGISTRATION_THREAD_IDENTIFIER			"zipNET::registration"

/*
 * Size of the largest expected message
 */
#define UI_MAXIMUM_MESSAGE_SIZE						8192

/*
 * Size of the largest expected title
 */
#define UI_COMPONENT_TITLE_MAXIMUM					1024

/*
 * Size of the largest expected path
 */
#define MAX_PATH  									512

/*
 * Declared in the UI unit
 */
extern bool UiInitialized;

/*
 * ID for a UI component
 */
typedef int32_t UiComponentID;

/*
 * ID for a listener
 */
typedef int32_t UiListenerID;

/*
 * A protocol message always starts with the header, the message id
 */
typedef uint32_t UiProtocolCommandID;
const UiProtocolCommandID UI_PROTOCOL_INITIALIZATION = 1;
const UiProtocolCommandID UI_PROTOCOL_CREATE_COMPONENT = 2;
const UiProtocolCommandID UI_PROTOCOL_REMOVE_COMPONENT = 3;
const UiProtocolCommandID UI_PROTOCOL_REMOVE_COMPONENT_MAP = 4;
const UiProtocolCommandID UI_PROTOCOL_ADD_COMPONENT = 5;
const UiProtocolCommandID UI_PROTOCOL_SET_TITLE = 6;
const UiProtocolCommandID UI_PROTOCOL_SET_GHOST_TITLE = 7;
const UiProtocolCommandID UI_PROTOCOL_GET_TITLE = 8;
const UiProtocolCommandID UI_PROTOCOL_SET_TITLE_FONT = 9;
const UiProtocolCommandID UI_PROTOCOL_SET_BOUNDS = 10;
const UiProtocolCommandID UI_PROTOCOL_SET_VISIBLE = 11;
const UiProtocolCommandID UI_PROTOCOL_SET_LISTENER = 12;
const UiProtocolCommandID UI_PROTOCOL_SET_NUMERIC_PROPERTY = 13;
const UiProtocolCommandID UI_PROTOCOL_GET_NUMERIC_PROPERTY = 14;
const UiProtocolCommandID UI_PROTOCOL_CANVAS_ACK_BUFFER_REQUEST = 15;
const UiProtocolCommandID UI_PROTOCOL_GET_BOUNDS = 16;
const UiProtocolCommandID UI_PROTOCOL_CANVAS_BLIT = 17;
const UiProtocolCommandID UI_PROTOCOL_REGISTER_DESKTOP_CANVAS = 18;
const UiProtocolCommandID UI_PROTOCOL_SET_PNG = 19;
const UiProtocolCommandID UI_PROTOCOL_SET_COMPONENT_COLOR = 20;
const UiProtocolCommandID UI_PROTOCOL_CHANGE_BACKGROUND = 21;
const UiProtocolCommandID UI_PROTOCOL_GET_RESOLUTION = 22;
const UiProtocolCommandID UI_PROTOCOL_SET_FOCUS = 23;
const UiProtocolCommandID UI_PROTOCOL_SET_FONT_SIZE = 24;
const UiProtocolCommandID UI_PROTOCOL_SET_TITLE_ALIGNMENT = 25;
const UiProtocolCommandID UI_PROTOCOL_REGISTER_TASK_MANAGER = 26;
const UiProtocolCommandID UI_PROTOCOL_SET_MOUSE_CURSOR_FORM = 27;

/*
 * Common status for requests
 */
typedef uint8_t UiProtocolStatus;
const UiProtocolStatus UI_PROTOCOL_SUCCESS = 0;
const UiProtocolStatus UI_PROTOCOL_FAIL = 1;

/*
 * Component types
 */
typedef uint32_t UiComponentType;
const UiComponentType UI_COMPONENT_TYPE_WINDOW = 0;
const UiComponentType UI_COMPONENT_TYPE_BUTTON = 1;
const UiComponentType UI_COMPONENT_TYPE_LABEL = 2;
const UiComponentType UI_COMPONENT_TYPE_TEXTFIELD = 3;
const UiComponentType UI_COMPONENT_TYPE_CANVAS = 4;
const UiComponentType UI_COMPONENT_TYPE_GEOSHAPE = 5;

/*
 * Types of events that can be listened to
 */
typedef uint32_t UiComponentEventType;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_ACTION = 0;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_BOUNDS = 1;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_CANVAS_WFA = 2; // "wait for acknowledge"-event
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_KEY = 3;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_FOCUS = 4;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_MOUSE = 5;
const UiComponentEventType UI_COMPONENT_EVENT_TYPE_CLOSE = 6;

/*
 *
 */
typedef uint8_t UiLayoutManager;
#define UI_LAYOUT_MANAGER_GRID		((UiLayoutManager) 0)
#define UI_LAYOUT_MANAGER_FLOW		((UiLayoutManager) 1)

/*
 *
 */
typedef struct
{
	UiProtocolCommandID id;
}__attribute__((packed)) UiMessageHeader;

/*
 * Request to initialize interface communications. The window server creates a
 * delegate thread that is responsible for further communications and responds
 * with a <UiInitializeResponse>.
 */
typedef struct
{
	UiMessageHeader header;
}__attribute__((packed)) UiInitializeRequest;

/*
 * Response for initializing interface communications.
 *
 * @field status
 * 		whether the initialization was successful
 * @field windowServerDelegateThread
 * 		id of the thread that is responsible for further window server communication
 */
typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
	Tid windowServerDelegateThread;
	Tid windowServerCleanUPThread;
}__attribute__((packed)) UiInitializeResponse;

/*
 * Request sent to create a component.
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentType type;
}__attribute__((packed)) UiCreateComponentRequest;

/*
 * Response when creating a component.
 */
typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
	UiComponentID id;
}__attribute__((packed)) UiCreateComponentResponse;

/*
 * Request to delete a component of current process.
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
} __attribute__((packed)) UiRemoveComponentRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiRemoveComponentResponse;

/*
 * Request / response to delete component map of current process.
 */
typedef struct
{
	UiMessageHeader header;
}__attribute__((packed)) UiRemoveComponentMapRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiRemoveComponentMapResponse;

/*
 * Request/response for adding a child
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID parent;
	UiComponentID child;
}__attribute__((packed)) UiComponentAddChildRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentAddChildResponse;

/*
 * Request/response for setting bounds
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	Rectangle bounds;
}__attribute__((packed)) UiComponentSetBoundsRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetBoundsResponse;

/*
 * Request/response for getting bounds
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
}__attribute__((packed)) UiComponentGetBoundsRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
	Rectangle bounds;
}__attribute__((packed)) UiComponentGetBoundsResponse;

/*
 * Request/response for setting components (in)visible
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	bool visible;
}__attribute__((packed)) UiComponentSetVisibleRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetVisibleResponse;

/*
 * Request/response for setting the title on a titled component
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	char title[UI_COMPONENT_TITLE_MAXIMUM];
}__attribute__((packed)) UiComponentSetTitleRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetTitleResponse;

/*
 *	request/response for setting ghost title on a titled component
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	char title[UI_COMPONENT_TITLE_MAXIMUM];
}__attribute__((packed)) UiComponentSetGhostTitleRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetGhostTitleResponse;

/*
 * Request/response for getting the title on a titled component
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
}__attribute__((packed)) UiComponentGetTitleRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
	char title[UI_COMPONENT_TITLE_MAXIMUM];
}__attribute__((packed)) UiComponentGetTitleResponse;


/*
 *	Request/response for set font on title
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	char fontName[MAX_PATH];
}__attribute__((packed)) UiComponentSetTitleFontRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetTitleFontResponse;

/*
 * Request/response for getting a numeric property
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	int property;
}__attribute__((packed)) UiComponentGetNumericPropertyRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
	uint32_t value;
}__attribute__((packed)) UiComponentGetNumericPropertyResponse;

/*
 * Request/response for setting a numeric property
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	int property;
	uint32_t value;
}__attribute__((packed)) UiComponentSetNumericPropertyRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetNumericPropertyResponse;


/*
 * Request/response for acknowledging the buffer update of a canvas/blitting the canvas
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
}__attribute__((packed)) UiComponentCanvasAckBufferRequest;

typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
}__attribute__((packed)) UiComponentCanvasBlitRequest;

/*
 * Canvas shared memory header
 */
typedef struct
{
	uint16_t paintableWidth;
	uint16_t paintableHeight;
	uint16_t blitX;
	uint16_t blitY;
	uint16_t blitWidth;
	uint16_t blitHeight;
}__attribute__((packed)) UiCanvasSharedMemoryHeader;

/*
 * Request to register the desktop canvas
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID canvasID;
}__attribute__((packed)) UiRegisterDesktopCanvasRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiRegisterDesktopCanvasResponse;

/*
 * Event handler registration functions
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	UiComponentEventType eventType;
	Tid targetThread;
}__attribute__((packed)) UiComponentSetListenerRequest;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetListenerResponse;

/*
 * Event structures
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentEventType type;
	UiComponentID componentID;
}__attribute__((packed)) UiComponentEventHeader;

typedef struct
{
	UiComponentEventHeader header;
}__attribute__((packed)) UiComponentActionEvent;

typedef struct
{
	UiComponentEventHeader header;
	Rectangle bounds;
}__attribute__((packed)) UiComponentBoundsEvent;

typedef struct
{
	UiComponentEventHeader header;
	Address newBufferAddress;
}__attribute__((packed)) UiComponentCanvasWfaEvent;

typedef struct
{
	UiComponentEventHeader header;
	Keyboard::InfoBasic keyInfo;
}__attribute__((packed)) UiComponentKeyEvent;

typedef struct
{
	UiComponentEventHeader header;
	uint8_t nowFocused;
}__attribute__((packed)) UiComponentFocusEvent;

/*
 *	component png setup/response
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;

	char pathToPng[MAX_PATH];
	Point pngPosition;
}__attribute__((packed)) UiComponentSetupPng;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentSetupPngResponse;

/*
 *	Component color setup/response
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;

	Color_t shapeColor;
	Color_t titleColor;
}__attribute__((packed)) UiComponentColor;

typedef struct
{
	UiMessageHeader header;
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentColorResponse;


/*
 *	change background
 */
typedef struct
{
	UiMessageHeader header;

	char path[MAX_PATH];
}__attribute__((packed)) UiChangeBackgroundRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiChangeBackgroundResponse;

/*
 *	send get resolution request
 */
typedef struct
{
	UiMessageHeader header;
}__attribute__((packed)) UiGetResolution;

/*
 *	get resolution of screen
 */
typedef struct
{
	UiProtocolStatus status;
	Dimension resolution;
}__attribute__((packed)) UiGetResolutionResponse;

/*
 *	close event of window
 */
typedef struct
{
	UiComponentEventHeader header;
}__attribute__((packed)) UiComponentCloseEvent;

/*
 *	request/response of focus event of component
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	bool focus;
}__attribute__((packed)) UiComponentFocusRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiComponentFocusResponse;

/*
 * request/response to set label font size
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	int size;
}__attribute__((packed)) UiSetFontSizeRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiSetFontSizeResponse;

/*
 *	request/reponse to set title alignment
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	TextAlignment alignment;
}__attribute__((packed)) UiSetTitleAlignmentRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiSetTitleAlignmentResponse;

/*
 *	request/response to register task manager on component
 */
typedef struct
{
	UiMessageHeader header;
	UiComponentID id;
	Rectangle bounds;
}__attribute__((packed)) UiRegisterTaskManagerRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiRegisterTaskManagerResponse;

/*
 * request/response to change form of mouse cursor
 */
typedef struct
{
	UiMessageHeader header;
	char name[50];
}__attribute__((packed)) UiSetMouseCursorFormRequest;

typedef struct
{
	UiProtocolStatus status;
}__attribute__((packed)) UiSetMouseCursorFormResponse;

/*
 * Mouse events
 */
typedef uint8_t MouseButton;
#define MOUSE_BUTTON_NONE		((MouseButton) 0x0)
#define MOUSE_BUTTON_1			((MouseButton) 0x1)
#define MOUSE_BUTTON_2			((MouseButton) 0x2)
#define MOUSE_BUTTON_3			((MouseButton) 0x4)

typedef uint8_t MouseEventType;
#define MOUSE_EVENT_NONE			((MouseEventType) 0)
#define MOUSE_EVENT_MOVE			((MouseEventType) 1)
#define MOUSE_EVENT_PRESS			((MouseEventType) 2)
#define MOUSE_EVENT_RELEASE			((MouseEventType) 3)
#define MOUSE_EVENT_DRAG_RELEASE	((MouseEventType) 4)
#define MOUSE_EVENT_DRAG			((MouseEventType) 5)
#define MOUSE_EVENT_ENTER			((MouseEventType) 6)
#define MOUSE_EVENT_LEAVE			((MouseEventType) 7)

/*
 *	mouse event data struct
 */
typedef struct
{
	UiComponentEventHeader header;
	Point position;
	MouseEventType type;
	MouseButton buttons;
}__attribute__((packed)) UiComponentMouseEvent;

#endif
