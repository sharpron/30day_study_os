
#include "layer.h"

struct LayerCtl* layerctl_init(struct MemMan *memman, struct Graphic *root_graphic) {
	struct LayerCtl *ctl = (struct LayerCtl*) memman_alloc(memman, 
		sizeof(struct LayerCtl));
	ctl->graphic = root_graphic;
	ctl->top = -1;

	for (int i = 0; i < MAX_LAYERS; i++) {
		ctl->layers_buf[i].used = false;
		ctl->layers_buf[i].ctl = ctl;
	}
	return ctl;
}

void layerctl_refresh(struct LayerCtl *ctl) {
	layerctl_refreshrange(ctl, 0, 0, ctl->graphic->width, ctl->graphic->height);
}

void layerctl_refreshrange(struct LayerCtl *ctl, int x, int y, int endx, int endy) {
	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (endx > ctl->graphic->width) endx = ctl->graphic->width;
	if (endy > ctl->graphic->height) endy = ctl->graphic->height;

	for (int i = 0; i <= ctl->top; i++) {
		struct Layer *layer = ctl->layers[i];
		struct Graphic *graphic = &layer->graphic;

		int x0 = x - layer->x;
		int y0 = y - layer->y;
		int x1 = endx - layer->x;
		int y1 = endy - layer->y;
		if (x0 < 0) x0 = 0;
		if (y0 < 0) y0 = 0;
		if (x1 > graphic->width) x1 = graphic->width;
		if (y1 > graphic->height) y1 = graphic->height;
		
		unsigned char *buf = graphic->buf;
		
		
		for (int by = y0; by < y1; by++) {
			int vy = layer->y + by;	
			for (int bx = x0; bx < x1; bx++) {
				int vx = layer->x + bx;
				unsigned char color = buf[by * graphic->width + bx];
				if (color != COL_TRANS) {
					setpixel(ctl->graphic, color, vx, vy);
				}
			}
		}
	}

}

struct Layer* layer_alloc(struct LayerCtl *ctl) {

	for (int i = 0; i < MAX_LAYERS; i++) {
		struct Layer *layer = &ctl->layers_buf[i];	
		if (!layer->used) {
			layer->used = true;
			layer->zindex = HIDDEN_ZINDEX;
			return layer;	
		}
	}
	return NULL;
}

void layer_setbuf(struct Layer *layer, unsigned char *buf,
    int width, int height) {
	layer->graphic.buf = buf;
	layer->graphic.width = width;
	layer->graphic.height = height;
}

void layer_refresh(struct Layer *layer) {
	if (layer->zindex > HIDDEN_ZINDEX) {
		layerctl_refreshrange(layer->ctl, layer->x, layer->y, 
			layer->x + layer->graphic.width, layer->y + layer->graphic.height);
	}
}

void layer_setzindex(struct Layer *layer, int zindex) {
	struct LayerCtl *ctl = layer->ctl;
	
	if (zindex + 1 > ctl->top) {
		zindex = ctl->top + 1;
	}
	else if (zindex < -1) {
		zindex = -1;
	}
	
	int old_zindex = layer->zindex;
	layer->zindex = zindex;

	if (old_zindex > zindex) {
		if (zindex >= 0) {
			for (int i = old_zindex; i > zindex; i++) {
				ctl->layers[i] = ctl->layers[i-1];
				ctl->layers[i]->zindex = i;
			} 
			ctl->layers[zindex] = layer;
		} else {
			if (ctl->top > old_zindex) {
				for (int i = old_zindex; i < ctl->top; i++) {
					ctl->layers[i] = ctl->layers[i+1];
                	ctl->layers[i]->zindex = i;
				}
			}
			ctl->top--;
		}
		layerctl_refresh(ctl);
	} 
	else if (old_zindex < zindex) {
		if (old_zindex >= 0) {
            for (int i = old_zindex; i < zindex; i++) {
                ctl->layers[i] = ctl->layers[i+1];
                ctl->layers[i]->zindex = i;
            }
        } else {
            for (int i = ctl->top; i >= zindex; i--) {
                ctl->layers[i + 1] = ctl->layers[i];
                ctl->layers[i + 1]->zindex = i + 1;
            }
            ctl->top++;
        }
        ctl->layers[zindex] = layer;
        layerctl_refresh(ctl);
	}
}

void layer_move(struct Layer *layer, int x, int y) {
	int lastx = layer->x;
	int lasty = layer->y;
	layer->x = x;
	layer->y = y;
	if (layer->zindex > HIDDEN_ZINDEX) {
		
		int layer_width = layer->graphic.width;
		int layer_height = layer->graphic.height;
	
		layerctl_refreshrange(layer->ctl, lastx, lasty, lastx + layer_width, 
			lasty + layer_height);

		layerctl_refreshrange(layer->ctl, x, y, x + layer_width, 
			y + layer_height);
	}
}


void layer_free(struct Layer *layer) {

	if (layer->zindex > HIDDEN_ZINDEX) {
		layer_setzindex(layer, HIDDEN_ZINDEX);
	}
	layer->used = false;

}
