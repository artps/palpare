//
//  gallery.cpp
//  Gallery
//
//  Created by Artyom Semyonov on 14.04.13.
//
//

#include "Gallery.h"


Gallery::Gallery(){

    ofAddListener(ofEvents().mouseMoved, this, &Gallery::mouseMoved);
	ofAddListener(ofEvents().mousePressed, this, &Gallery::mousePressed);
	ofAddListener(ofEvents().mouseDragged, this, &Gallery::mouseDragged);
	ofAddListener(ofEvents().mouseReleased, this, &Gallery::mouseReleased);
    
    m_options.x = 0;
    m_options.y = 0;
    m_options.width  = ofGetWindowWidth();
    m_options.height = ofGetWindowHeight();

    m_options.thumb_width  = m_options.width * 0.3;
    m_options.thumb_height = m_options.height * 0.3;

    m_options.selected_scale = 0.8;
    m_options.resistence = 0.1;
    m_options.state = VERTICAL_ALIGN;
    
    m_speed_threshold = .01f;
    
    m_selected       = 0;
    m_move_selection = 0;
    
	b_debug    = false;
    b_activity = false;
}

Gallery::~Gallery(){
	for(int i = 0; i < m_items.size() ;i++) {
		delete m_items[i];
    }
    
	m_items.clear();
}

Gallery& Gallery::setPosition(int x, int y) {
    m_options.x = x;
    m_options.y = y;
    return * this;
}

Gallery& Gallery::setSize(int width, int height){
    m_options.width = width;
    m_options.height = height;
    m_options.thumb_width = m_options.width * 0.3;
    m_options.thumb_height = m_options.height * 0.3;
    return * this;
}

Gallery& Gallery::setMode(State state){
    m_options.state = state;
    
    if (m_items.size() > 0){
        if (m_options.state == HORIZONTAL_ALIGN){
            m_items[0]->setPosition(
                ofVec2f(m_options.x + 0,
                        m_options.y + m_options.height * 0.5));
        }
        
        for(int i = 0; i < m_items.size(); i++){
            m_items[i]->setState(state);
            m_items[i]->m_back_looking = true;
        }
        
        repairLinks();
        
        setSelected(m_selected);
    }
}

void Gallery::loadDir(string _dirPath){
	ofDirectory dir;

    dir.allowExt("jpg");
    dir.allowExt("JPG");
	dir.allowExt("jpeg");
    dir.allowExt("JPEG");
    dir.allowExt("png");
    dir.allowExt("PNG");
    dir.allowExt("gif");
    dir.allowExt("GIF");
    

	int photos_num = dir.listDir(_dirPath);

	for(int i = 0; i < photos_num;i++){
        loadPhoto(dir.getPath(i));
    }
}

void Gallery::loadPhoto(string file_path){
    GalleryItem * item = new GalleryItem();
    
    item->setImage(file_path);
    item->setPosition(ofVec2f(ofRandom(m_options.x, m_options.width),
                              ofRandom(m_options.y, m_options.height)));
    item->setState(m_options.state);
    item->setGalleryOptions(&m_options);
    
    if (m_items.size() > 0) {
        item->m_prev = m_items.back();
        m_items.back()->m_next = item;
    } else {
        item->m_prev =NULL;
    }
    

    m_items.push_back(item);
}

void Gallery::remove( int photo_id){
    if ( (photo_id >= 0 ) && (photo_id < m_items.size() )){
        delete m_items[photo_id];
        m_items.erase(m_items.begin() + photo_id);
        repairLinks();
    }
}

void Gallery::pushToFront(int photo_id ){
    
	if ( (photo_id >= 0 ) && (photo_id < m_items.size() )){
        
		GalleryItem * tmp = m_items[photo_id];
        
		for (int i = photo_id; i < m_items.size() - 1; i++){
			m_items[i] = m_items[i + 1];
            
            if (i > 0){
                m_items[i]->m_prev = m_items[i-1];
                m_items[i]->m_prev->m_next = m_items[i];
            }
            
		}
		
        m_items.back() = tmp;
        m_items.back()->m_prev = m_items[m_items.size() - 2];
        m_items.back()->m_prev->m_next = m_items.back();

        m_items[0]->m_prev = NULL;
        m_items.back()->m_next = NULL;
        
        //repairLinks();
	}
}

void Gallery::repairLinks(){
    if ( m_items.size() > 0){
        
        for (int i = 0; i < m_items.size(); i++){
            if (i > 0){
                m_items[i]->m_prev = m_items[i - 1];
                m_items[i]->m_prev->m_next = m_items[i];
            }
        }
        
        m_items[0]->m_prev = NULL;
        m_items[m_items.size() - 1]->m_next = NULL;
    }
}

void Gallery::setSelected(int photo_id){
    if ((photo_id >= 0) && ( photo_id < m_items.size()) ){
        for (int i = 0; i < photo_id; i++ ){
            m_items[i]->setState( m_options.state );
            m_items[i]->m_back_looking = false;
        }
        
        m_selected = photo_id;
        m_items[photo_id]->setState(CENTER);
        m_items[photo_id]->m_back_looking = false;
        
        for (int i = photo_id + 1; i < m_items.size() ; i++ ){
            m_items[i]->setState( m_options.state );
            m_items[i]->m_back_looking = true;
        }
    }
}

void Gallery::moveFoward(){
    if (m_selected < m_items.size()){
        setSelected(m_selected + 1);
    }
}

void Gallery::moveBackward(){
    if (m_selected > 0) {
        setSelected(m_selected - 1);
    }
}


void Gallery::update(){    
    if (m_options.state == HORIZONTAL_ALIGN && m_move_selection != 0){
        if ( !b_activity ){
            if ( m_move_selection == -1) {
                moveBackward();
            } else if ( m_move_selection == 1) {
                moveFoward();
            }
            m_move_selection = 0;
        }
	}

	for(int i = 0; i < m_items.size(); i++){
		m_items[i]->update();
	}
}

void Gallery::draw(){
	for(int i = 0; i < m_items.size(); i++){
		m_items[i]->draw();
	}
    
}

void Gallery::mouseMoved(ofMouseEventArgs &e){
    ofVec2f mouse = ofVec2f(e.x, e.y);
    m_mouse_last = mouse;
}

void Gallery::mousePressed(ofMouseEventArgs &e){
    ofVec2f mouse = ofVec2f(e.x, e.y);
    
    if ( m_options.state == FREE ){
        for(int i = m_items.size() - 1; i >= 0; i--){
            if (m_items[i]->isOver(mouse)){
                pushToFront(i);
                break;
            }
        }
    }
    m_mouse_last = mouse;
    b_activity  = true;
}

void Gallery::mouseDragged(ofMouseEventArgs &e){
    ofVec2f mouse = ofVec2f(e.x, e.y);
    ofVec2f mouse_velocity = mouse - m_mouse_last;
    
    if( (mouse_velocity.x <= -1) && ( m_move_selection == 0) ){
        m_move_selection = 1;
    } else if ((mouse_velocity.x >= 1 ) && ( m_move_selection == 0)){
        m_move_selection = -1;
    }
        

    b_activity = true;
}

void Gallery::mouseReleased(ofMouseEventArgs &e){
    b_activity = false;
}
