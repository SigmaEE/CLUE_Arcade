#define _XOPEN_SOURCE 600
#define _GNU_SOURCE
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xatom.h>

#include <stdio.h>
#include <stdlib.h>		/* getenv(), etc. */
#include <unistd.h>		/* sleep(), etc.  */
#include <time.h>
#include <math.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>

#define PIXEL_AREA_WIDTH 30
#define PIXEL_AREA_HEIGHT 60
#define PSIZE_MAX 15
#define PSIZE_MIN 1
#define PSIZE_START 5

#define WWIDTH (30 * 5)
#define WHEIGHT (60 * 5)

Display *display;
Visual *visual;
Window win;
int screen_num;
XImage * ximages[15] = { [ 0 ... 14 ] = NULL };
char * data = NULL;

sem_t draw_done_sem;
sem_t start_draw_sem;
int draw_counter = 0;
void set_resize_hints(Window * win)
{
	XSizeHints * sizehints = XAllocSizeHints();

	sizehints->min_width = 30;
	sizehints->min_height = 60;
	sizehints->max_width = 450;
	sizehints->max_height = 900;
	sizehints->width_inc = 1;
	sizehints->height_inc = 60;
	sizehints->min_aspect.x = 450;
	sizehints->min_aspect.y = 900;
	sizehints->max_aspect.x = 1;
	sizehints->max_aspect.y = 1;
	sizehints->flags = PMinSize | PMaxSize | PResizeInc | PAspect;
	XSetWMNormalHints(display, *win, sizehints); //, XA_WM_SIZE_HINTS);
}

XImage * get_im(int psize)
{
	XImage ** im = &ximages[psize - 1]; //  lowest pixel size is 1, therefore subtract 1 to reach index 0

	if (*im == NULL) {
		*im = XCreateImage(display, visual, DefaultDepth(display, screen_num),ZPixmap,0, data, psize * 30, psize * 60, 32, 0);
	}
	return *im;
}

void put_pixel(XImage * im, int psize, int x, int y, long col)
{
	for (int _x = x * psize; _x < (x + 1) * psize; ++_x) {
		for (int _y = y * psize; _y < (y + 1) * psize; ++_y) {
			//XPutPixel(im, _x, _y, col);
			*(int*)&im->data[4 * _y * psize * 30 + _x * 4] = col;
		}
	}
	/*
	int c = psize + 1;
	int ylen = 29 * psize;
	int * ptr = (int*)  &im->data[((y * psize * 30 +  x) * psize) << 2];
	while (c--) {
		int * tmp = &ptr[psize];
		while (ptr < tmp) {
		      *ptr++ = col;
		}
		ptr += ylen;
		//ptr -= psize;

	}
	*/
}

int millis_since(struct timespec * t1)
{
  // return delta in milliseconds
  struct timespec time;
  clock_gettime(CLOCK_REALTIME, &time);

  int delta = 1000 * (time.tv_sec - t1->tv_sec);
  delta += (time.tv_nsec - t1->tv_nsec) / (1000 * 1000);;
  return delta;
}


int global_psize = 5;
int get_psize(void)
{
	//int dummy; int win_w; int win_h; Window dummyw;
	//XGetGeometry(display, win, &dummyw, &dummy, &dummy, &win_w, &win_h, &dummy, &dummy);
	//win_h = 300;
	//return win_h / 60;
	return global_psize;
}

void draw_plasma_v1(XImage * im, int psize, int ofs)
{
	for (int x = 0; x < 30; x++) {
		for (int y = 0; y < 60; y++) {
			int r = ((255 * (x)) / 30) << 0;
			int b = ((255 * y) / 60) << 8;
			int gr = ((255 * (60-y)) / 60) << 16;
			put_pixel(im, psize, x, y, r + b + gr);

			double b1 = 1+sin(x * 1.8 + ofs  * 0.19 + y * 1.2);
			double b2 = 1+sin(x * 1.5 + ofs  * 0.8 + y * 1.4);
			double b3 = 1+sin((2 + y * x * ofs * 0.1) + (1 +  ofs * 0.01) + y * 0.1);
			b1 = 1 + sin(x * 0.15 + ofs * 0.1);
			b2 = 1 + sin(x * 0.009 + y * 0.07);
			b3 = 1 + sin(x * 0.03 + y * 0.001 + ofs * 0.1);
			b1 = 1 + sin(x / (10 * sin(ofs * 0.01)));
			//b3 = 1 + sin(y * 0.1);
			b3 = 0;
			double bp = 20 * b1 + 40 * b2 + 20 * b3;
			double color = (int)((bp)) << 0;
			//color       += (int)(20 * (r1 + r2 + r3)) << 8;
			put_pixel(im, psize, x, y, color);
		}
		//int y = 30 + 10 * sin((x + ofs) / 5.0);
		//int y = 30 + 10 * sin( (ofs / 20.0) + (x) * (30.0 / (double)ofs * 0.1));
	}
}

#define RANGE(from, to) ( (c >= from && c < to) * (c - from) )
long plasma_grad(double c)
{
	int r, g, b;

	b = 55 + RANGE(0.0, 1.0) * 200;
	r = RANGE(0.4, 1.0) * 200;
	g = RANGE(0.8, 1.0) * 255;

	b = b * 0.5;
	r = r * 0.5;
	g = g * 0.5;
	//r = (c > 0.4 && c < 0.43) * ((c - 0.4) / 0.03) * 255;
	//g = (c > 0.6 && c < 0.63) * 255;
	//b = (c > 0.8 && c < 0.83) * 255;
	//b = g = 0;
	return b + (g << 8) + (r << 16);
}

extern int fastled_leds[60 * 30];
void draw_leds(void)
{
	int psize = get_psize();
	XImage *im = get_im(psize);
	int y = 0;
	for (y = 0; y < 58; ++y) {
		for (int x = 0; x < 30; ++x) {
			int tx = (y & 1) ? x+1 : 30 - x;
			put_pixel(im,  psize, x, y + 2, fastled_leds[y*30+tx]);
		}
	}
}

int joystickX = 0;
int joystickY = 0;

void setup(void);
void loop(void);
void * emu_thread(void * arg)
{
	int i = 0;
	struct timespec delay = { 0, 1000 * 1000 * 5 };
	struct timespec left;
	setup();
	while (1) {
		loop(); // arduino loop
		nanosleep(&delay, &left);
	}
}

void * draw_thread(void * arg)
{
	static int last_draw = 0;
	while(1) {
		//XNextEvent(display,&event);
		//if (millis_since(&draw_time) > 20) {
		sem_wait(&start_draw_sem);
		if (draw_counter > last_draw) {
			static double ofs = 0;
			ofs += 1;

			int psize = get_psize();
			int w = psize * 30;
			int h = psize * 60;
			XImage *im = get_im(psize);

			draw_leds();
			XPutImage(display, win, DefaultGC(display,screen_num), im, 0, 0, 0, 0, w, h);

			sem_post(&draw_done_sem);
		}
	}
}

int main(int argc, char *argv[])
{
	//draw();

	XInitThreads();
	display = XOpenDisplay(NULL);
	screen_num = DefaultScreen(display);
	visual = DefaultVisual(display, screen_num);
	Window root = RootWindow(display, screen_num);
	win = XCreateSimpleWindow(display, root, 0, 0, WWIDTH, WHEIGHT, 1, 0, 0);
	set_resize_hints(&win);
	// Window Class
	XClassHint ch = {"CLUE arcade", "CLUE arcade"};
	XSetClassHint(display, win, &ch);
	// Window Title
	char * s[2] = { "CLUE arcade", NULL };
	XTextProperty xtp;
	XStringListToTextProperty(s, 2, &xtp);
	XSetWMName(display, win, &xtp);

	data = (char*)malloc(900 * 450 * 4); // alloc for max size XXX could realloc in get_im instead

	XSelectInput(display, win, ExposureMask | ButtonPressMask | KeyPressMask | KeyReleaseMask | StructureNotifyMask);

	XMapWindow(display,win);

	pthread_attr_t pt_attrs;
	pthread_attr_t pt_attrs_2;
	pthread_attr_init(&pt_attrs);
	pthread_attr_init(&pt_attrs_2);
	pthread_t emu_pt;
	pthread_t draw_pt;
	pthread_create(&emu_pt, &pt_attrs, emu_thread, NULL);
	pthread_create(&draw_pt, &pt_attrs_2, draw_thread, NULL);

	struct timespec start_time;
	struct timespec draw_time;
	clock_gettime(CLOCK_REALTIME, &start_time);
	clock_gettime(CLOCK_REALTIME, &draw_time);
	XEvent event;
	while(1) {
		int got_event = 1;
		while (1 == got_event) {
			/*
			got_event = XCheckMaskEvent(display, ExposureMask | KeyPressMask | KeyReleaseMask| ButtonPressMask | StructureNotifyMask, &event);
			if (!got_event) {
				continue;
			}
			*/
			int redraw = 0;
			XNextEvent(display, &event);
			if(event.type == Expose)
			{
				int h = event.xexpose.height;
				int w = event.xexpose.width;
				if (event.xexpose.count == 0) {
					// check aspect ratio
					if ((2 * w) > h) { // width is too big in relation to height
						w = h / 2;
					}
					if ((2 * w) < h) { // width is too small in relation to height
						h = w * 2;
					}
					// XXX TODO maybe just draw grey in areas outside main pixbuf
					int psize = w / 30;
					global_psize = psize;

					redraw = 1;
					printf("w:%d h:%d psize:%d\n", w, h, psize);
					//XImage *im = get_im(psize);
					/*
					   for (int i = 0; i < 30; i++) {
					   for (int j = 0; j < 60; j++) {
					   int r = ((255 * i) / 30) << 0;
					   int b = ((255 * j) / 60) << 8;
					   int g = ((255 * (60-j)) / 60) << 16;
					   put_pixel(im, psize, i, j, r + b + g);
					   }
					   }
					   */
					//XPutImage(display, win, DefaultGC(display,screen_num), im, 0, 0, 0, 0, w, h);
				}
			}

			if (event.type == KeyPress) {
				//printf("Keycode %d\n", event.xkey.keycode);
				if (event.xkey.keycode == 53) { // 'q'
					exit(0);;
				}
				if (event.xkey.keycode == 111) {
					joystickY = -1;
				}
				if (event.xkey.keycode == 116) {
					joystickY = 1;
				}
				if (event.xkey.keycode == 113) {
					joystickX = -1;
				}
				if (event.xkey.keycode == 114) {
					joystickX = 1;
				}
			}
			if (event.type == KeyRelease) {
				//printf("keyrelease\n");
				if (event.xkey.keycode == 111) {
					joystickY = 0;
				}
				if (event.xkey.keycode == 116) {
					joystickY = 0;
				}
				if (event.xkey.keycode == 113) {
					joystickX = 0;
				}
				if (event.xkey.keycode == 114) {
					joystickX = 0;
				}
			}
			if (event.type == ConfigureNotify) {
				int h = event.xconfigure.height;
				int w = event.xconfigure.width;
				if (h != 2 * w) {
					w = h / 2;
				}
			}
			if (event.type == ButtonPress) {
				int psize = get_psize();
				printf("buttonpress %d\n", event.xbutton.button);
				/*
				if (event.xbutton.button == 1) { // mouse button 1
					exit(0);
				}
				*/
				if (event.xbutton.button == 4 && psize < PSIZE_MAX) { // scrollwheel up
					psize++;
				} else if (event.xbutton.button == 5 && psize > PSIZE_MIN) { // scrollwheel down
					psize--;

				} else {
					continue;
				}
				printf("resizing %d %d\n", psize * PIXEL_AREA_WIDTH, psize * PIXEL_AREA_HEIGHT);
				XResizeWindow(display, win, psize * PIXEL_AREA_WIDTH, psize * PIXEL_AREA_HEIGHT);
				global_psize = psize;
			}
			if (redraw) {
				sem_post(&start_draw_sem);
				sem_wait(&draw_done_sem);
			}
		}
	}

	XCloseDisplay(display);
	return 0;
}
