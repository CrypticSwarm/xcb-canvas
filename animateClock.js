var xcb = require('xcbjs')
  , Canvas = require('canvas')
  , clock = require('./clock')
  , canvas = new Canvas(320, 320)
  , ctx = canvas.getContext('2d')

new xcb.Connection(function(display) {
  var X = this
    , screen = X.getScreen()
    , wid = X.generateId()
    , gc  = X.generateId()
  X.CreateWindow(
    { depth: 0
    , wid: wid
    , parent: screen.root
    , x: 10
    , y: 10
    , width: 320
    , height: 320
    , border_width: 1
    , _class: 1
    , visual: 0
    , value_mask: 2050
    , value_list: [screen.white_pixel, 32768]
  });
  X.MapWindow({ window: wid });

  function getMainVisual() {
    var rootVis = screen.root_visual
      , vis
    screen.allowed_depths.some(function(depth) {
      return depth.visuals.some(function(visual) {
        return vis = (visual.visual_id == rootVis ? visual : null)
      })
    })
    return vis
  }


  xcb.onExpose = drawIt
  var vis = getMainVisual()
    , xcbCanvas = require('./xcb-canvas')
  drawIt()
  setInterval(drawIt, 10)
  function drawIt() {
    clock(ctx)
    xcbCanvas.displayCanvas.call(X, wid, vis, canvas)
    X.flush()
  }
})
