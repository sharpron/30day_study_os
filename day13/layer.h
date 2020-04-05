
#include "graphic.h"
#include "mem.h"
#include <stddef.h>
#include <stdbool.h>

#define HIDDEN_ZINDEX	-1
#define MAX_LAYERS 		200

/* 层 */
struct Layer {
	struct Graphic graphic;
	int	x, y, zindex;
	bool used;
};

/* 对层的管理信息 */
struct LayerCtl {
	int top;
	// 根图形
	struct Graphic *graphic;
	struct Layer *layers[MAX_LAYERS];
	struct Layer layers_buf[MAX_LAYERS];
};

/* 初始化一个层的控制器 */
struct LayerCtl* layerctl_init(struct MemMan *memman, struct Graphic *root_graphic);

/* 刷新层控制器 */
void layerctl_refresh(struct LayerCtl *ctl);
 
/* 从层管理器中获得一个未使用的层 */
struct Layer* layer_alloc(struct LayerCtl *ctl);

/* 初始化层 */
void layer_setbuf(struct Layer *layer, unsigned char *buf, 
	int width, int height);

/* 设置zindex的方向 */
void layer_setzindex(struct LayerCtl *ctl, struct Layer *layer, int zindex);

/* 移动指定层 */
void layer_move(struct LayerCtl *ctl, struct Layer *layer, int x, int y);

/* 让层回到空闲状态 */
void layer_free(struct LayerCtl *ctl, struct Layer *layer);
