#include <stdio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_primitives.h>;
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>

typedef struct {
	int x;
	int y;
	int w;
	double h;
	ALLEGRO_COLOR color;

}Bar;



int main(int args, char** argv) {

	Bar bars[43];
	Bar bars2[43];
	Bar bars3[43];

	int w = 860 / 43;

	FILE* file;
	errno_t err;
	if (err = fopen_s(&file, "output_values_c_avg.txt", "r") != 0) {
		exit(-1);
	}

	FILE* file2;
	errno_t err2;
	if (err2 = fopen_s(&file2, "output_values.txt", "r") != 0) {
		exit(-1);
	}

	FILE* file3;
	errno_t err3;
	if (err3 = fopen_s(&file3, "output_values_c_avg2.txt", "r") != 0) {
		exit(-1);
	}

	for (int i = 0; i < 43; i++) {
		bars[i].x = i * w;
		bars[i].y = 600;
		bars[i].w = w-2;
		bars[i].h = 600-10;
		bars[i].color = al_map_rgb(0, 127, 255);

		bars2[i].x = i * w;
		bars2[i].y = 600;
		bars2[i].w = w - 2;
		bars2[i].h = 600 - 10;
		bars2[i].color = al_map_rgb(0, 127, 255);

		bars3[i].x = i * w;
		bars3[i].y = 600;
		bars3[i].w = w - 2;
		bars3[i].h = 600 - 10;
		bars3[i].color = al_map_rgb(0, 127, 255);
	}







	bool visualizer_loop = true;
	
	ALLEGRO_DISPLAY* display;
	ALLEGRO_EVENT_QUEUE* queue;
	ALLEGRO_EVENT evt;
	ALLEGRO_TIMEOUT timeout;
	ALLEGRO_TIMER* timer;
	ALLEGRO_SAMPLE* sample;


	al_init();
	al_init_primitives_addon();
	al_init_timeout(&timeout, 0.06);
	al_init_acodec_addon();
	al_install_audio();
	al_reserve_samples(1);
	display = al_create_display(860, 600);
	queue = al_create_event_queue();
	timer = al_create_timer(1.0 / 43.0);


	al_register_event_source(queue,al_get_display_event_source(display));
	al_register_event_source(queue,al_get_timer_event_source(timer));
	al_start_timer(timer);

	sample = al_load_sample("test6.wav");

	while (visualizer_loop) {
		al_play_sample(sample, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
		bool event_rec = al_wait_for_event_until(queue, &evt, &timeout);
		if (event_rec && evt.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {
			visualizer_loop = false;
		}
		else if (event_rec && evt.type == ALLEGRO_EVENT_TIMER) {
			double rec;
			if (fscanf_s(file, "%lf", &rec) != EOF) {
				for (int i = 42; i > 0; i--) {
					bars[i].h = bars[i - 1].h;
				}
				bars[0].h = 600 - rec*0.7;
			}
			if (fscanf_s(file2, "%lf", &rec) != EOF) {
				for (int i = 42; i > 0; i--) {
					bars2[i].h = bars2[i - 1].h;
				}
				bars2[0].h = 600 - rec*0.7;
			}
			if (fscanf_s(file3, "%lf", &rec) != EOF) {
				for (int i = 42; i > 0; i--) {
					bars3[i].h = bars3[i - 1].h;
				}
				bars3[0].h = 600 - rec*0.7;
			}
			al_clear_to_color(al_map_rgb(0, 0, 0));
			for (int i = 0; i < 43; i++) {
				al_draw_filled_rectangle(bars[i].x + 2, bars[i].h, bars[i].x + bars[i].w, bars[i].y,al_map_rgb(165,27,11));
				al_draw_filled_rectangle(bars3[i].x + 2, bars3[i].h, bars3[i].x + bars3[i].w, bars3[i].y, al_map_rgb(230, 28, 102));
				al_draw_filled_rectangle(bars2[i].x + 2, bars2[i].h, bars2[i].x + bars2[i].w, bars2[i].y, al_map_rgb(20, 100, 220));
				
			}
			al_flip_display();
		}
	}
	fclose(file);
	fclose(file2);
	fclose(file3);
	al_destroy_event_queue(queue);
	al_destroy_timer(timer);
	al_destroy_display(display);

	return 0;
}