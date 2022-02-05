extern crate sdl2;
extern crate gl;

use sdl2::event::Event;
use sdl2::keyboard::Keycode;
use sdl2::video::GLProfile;
use std::thread;

const WINDOW_TITLE: &str = "Hello Window";

unsafe fn render() {
  gl::ClearColor(0.6, 0.0, 0.0, 1.0);
  gl::Clear(gl::COLOR_BUFFER_BIT);
}

fn main() {
  let sdl_context = sdl2::init().unwrap();
  let video_subsystem = sdl_context.video().unwrap();
  
  let gl_attr = video_subsystem.gl_attr();
  gl_attr.set_context_profile(GLProfile::Core);
  gl_attr.set_context_version(3, 3);

  let window = video_subsystem.window(WINDOW_TITLE, 800, 600)
      .opengl()
      .build()
      .unwrap();

  // Unlike the other example above, nobody created a context for your window, so you need to create one.
  let ctx = window.gl_create_context().unwrap();
  gl::load_with(|name| video_subsystem.gl_get_proc_address(name) as *const _);
  
  debug_assert_eq!(gl_attr.context_profile(), GLProfile::Core);
  debug_assert_eq!(gl_attr.context_version(), (3, 3));

  let mut event_pump = sdl_context.event_pump().unwrap();

  'running: loop {
      unsafe {
        render();
      }

      window.gl_swap_window();
      for event in event_pump.poll_iter() {
          match event {
              Event::Quit {..} | Event::KeyDown { keycode: Some(Keycode::Escape), .. } => {
                  break 'running
              },
              _ => {}
          }
      }
      ::std::thread::sleep(::std::time::Duration::new(0, 1_000_000_000u32 / 60));
  }

  /*
  let sdl = SDL::init(InitFlags::Everything).expect("couldn't start SDL");
  sdl.gl_set_attribute(SdlGlAttr::MajorVersion, 3).unwrap();
  sdl.gl_set_attribute(SdlGlAttr::MinorVersion, 3).unwrap();
  sdl.gl_set_attribute(SdlGlAttr::Profile, GlProfile::Core).unwrap();
  #[cfg(target_os = "macos")]
  {
    sdl
      .gl_set_attribute(SdlGlAttr::Flags, ContextFlag::ForwardCompatible)
      .unwrap();
  }


  // Underscores a rust convention for local vars?
  let _win = sdl
    .create_gl_window(
      WINDOW_TITLE,
      WindowPosition::Centered,
      800,
      600,
      WindowFlags::Shown,
    )
    .expect("couldn't make a window and context");

    

    //let sdl_thread_context = _win

  // Spawn renderer and game loop in separate threads.
  let context;

  'main_loop: loop {
    // handle events this frame
    // poll events sticks the events on to a 
    while let Some(event) = sdl.poll_events().and_then(Result::ok) {
      match event {
        Event::Quit(_) => break 'main_loop,
        _ => (),
      }
    }
  }*/
}