//
//  gallery.h
//  Gallery
//
//  Created by Artyom Semyonov on 14.04.13.
//
//

#include "GalleryItem.h"

class Gallery {
public:
	Gallery();
    ~Gallery();
    
    Gallery& setPosition(int x, int y);
    Gallery& setSize(int width, int height);
    Gallery& setMode(State state);

    void loadDir(string dir_path);
    void loadPhoto(string file_path);
    
    void remove(int photo);
    void setSelected(int photo);
    void pushToFront(int photo);

    void moveFoward();
    void moveBackward();
    
    int  getSelected(){ return m_selected; };
    
    void repairLinks();
    void update();
    void draw();
    
    bool b_debug;
    
private:

	void mouseMoved(ofMouseEventArgs &e);
    void mousePressed(ofMouseEventArgs &e);
    void mouseDragged(ofMouseEventArgs &e);
    void mouseReleased(ofMouseEventArgs &e);
    
    Options     m_options;
    
	vector<GalleryItem*> m_items;

    ofVec2f     m_mouse_last;
    
    float       m_speed_threshold;
    
    int         m_selected;       // Hold the selected photo
    int         m_move_selection;  // This is for the slider in order to know if a moving have to be done.
    
    bool        b_activity;
};