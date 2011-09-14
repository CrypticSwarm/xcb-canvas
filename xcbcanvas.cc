#include <node.h>
#include <iostream>
#include <xcb/xcb.h>
#include <cairo/cairo-xcb.h>
#include "XCBJS/src/xcb.h"
#include "XCBJS/src/__autogen_structs.h"
#include "node-canvas/src/Canvas.h"


using namespace std;
using namespace v8;
using namespace node;

class XCBCanvas {

public:
  static void Init(Handle<Object> target) {
    HandleScope scope;
    NODE_SET_METHOD(target, "displayCanvas", XCBCanvas::displayCanvas);
  }

  static Handle<Value> displayCanvas(const Arguments& args) {
    HandleScope scope;
    xcb_visualtype_t vis;
    fromJS(args[1]->ToObject(), &vis);
    Canvas *canvas = ObjectWrap::Unwrap<Canvas>(args[2]->ToObject());
    cairo_surface_t *source = canvas->surface();
    cairo_surface_t *dest = cairo_xcb_surface_create(ObjectWrap::Unwrap<XCBJS::XCBJS>(args.This())->getConnection(), args[0]->IntegerValue(), &vis, 400, 400);
    cairo_t *cairo = cairo_create(dest);
    cairo_set_source_surface(cairo, source, 0.0, 0.0);
    cairo_paint(cairo);
    cairo_destroy(cairo);
    cairo_surface_destroy(dest);

    return Undefined();
  }
 
};

extern "C" void
init (v8::Handle<v8::Object> target)
{
  v8::HandleScope scope;
  XCBCanvas::Init(target);
}

