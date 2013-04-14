//
//  GalleryItem.h
//  Gallery
//
//  Created by Artyom Semyonov on 14.04.13.
//
//

#include "ofMain.h"
#include <vector>

struct tCursor {
	int	idN;
	ofVec2f loc;
};

enum State {
	FREE,
    VERTICAL_ALIGN,
    HORIZONTAL_ALIGN,
    CENTER
};

struct Options {
	float   x, y, width, height, thumb_width, thumb_height, resistence, selected_scale;
    State   state;
};

class GalleryItem {

    public:
	GalleryItem();
	
	GalleryItem& setPosition(ofVec2f value);
	GalleryItem& setImage(string image_file_path);
    GalleryItem& setState(State value);
    GalleryItem& setGalleryOptions(Options *value);
    
    GalleryItem& setPrev(GalleryItem *item);
    GalleryItem& setNext(GalleryItem *item);
    GalleryItem * getPrev();
    GalleryItem * getNext();

    void rotate(float angle);
	void resize(float resize);
    void move(ofVec2f force);
    
    ofVec2f getPosition();

    float getWidth();
    float getHeight();
    float getScaledWidth();
    float getScaledHeight();
    
	void    update();
	void    draw();
	
	bool    isOver(ofVec2f locaction);
	bool    isOver(int x, int y);

    GalleryItem      *m_prev;
    GalleryItem      *m_next;

    bool            m_back_looking;

    private:
    ofImage         m_img;

    Options         *m_gallery;

	vector<tCursor>	m_cursors_on_border;
	ofVec2f         m_old_location[3];
    ofVec2f         m_position, m_velocity, m_accelerate, m_size;

	float           m_angle, m_scale;
    State           m_state;
};
