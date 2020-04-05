
#include "layer.h"

struct LayerCtl* layerctl_init(struct MemMan *memman, struct Graphic *root_graphic) {
	struct LayerCtl *ctl = memman_alloc(memman, sizeof(struct LayerCtl));
	ctl->graphic = root_graphic;
	ctl->top = -1;
	return ctl;
}

void layerctl_refresh(struct LayerCtl *ctl) {
	for (int i = 0; i <= ctl->top; i++) {
		struct Layer *layer = ctl->layers[i];
		struct Graphic *graphic = &layer->graphic;
		unsigned char *buf = graphic->buf;
	
		for (int y = 0; y < graphic->height; y++) {
			int vy = layer->y + y;
			for (int x = 0; x < graphic->width; x++) {
				int vx = layer->x + x;
				unsigned char color = buf[y * graphic->width + x];
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

void layer_setzindex(struct LayerCtl *ctl, struct Layer *layer, int zindex) {

	
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

void layer_move(struct LayerCtl *ctl, struct Layer *layer, int x, int y) {
	layer->x = x;
	layer->y = y;	
	if (layer->zindex > HIDDEN_ZINDEX) {
		layerctl_refresh(ctl);
	}
}


void layer_free(struct LayerCtl *ctl, struct Layer *layer) {

	if (layer->zindex > HIDDEN_ZINDEX) {
		layer_setzindex(ctl, layer, HIDDEN_ZINDEX);
	}
	layer->used = false;

}
