//
//  GalleryItem.cpp
//  Gallery
//
//  Created by Artyom Semyonov on 14.04.13.
//
//

#include "GalleryItem.h"

GalleryItem::GalleryItem(){
	m_angle = 0;
	m_position.set(0, 0);
	m_size.set(0, 0);
	m_scale = 1;
    m_state = HORIZONTAL_ALIGN;
    
    m_back_looking = true;
    
    m_prev = NULL;
    m_next = NULL;
    m_gallery = NULL;
}

GalleryItem& GalleryItem::setPosition(ofVec2f value){
    m_position = value;
    return * this;
}

GalleryItem& GalleryItem::setImage(string image_file_path){
	m_img.loadImage(image_file_path);
    m_size.set(m_img.getWidth(), m_img.getHeight());
    return * this;
}

GalleryItem& GalleryItem::setState(State value){
    m_state = value;
    return * this;
}

GalleryItem& GalleryItem::setGalleryOptions(Options *value){
    m_gallery = value;
    return * this;
};

GalleryItem& GalleryItem::setPrev(GalleryItem *item){
    m_prev = item;
    return * this;
}

GalleryItem& GalleryItem::setNext(GalleryItem *item){
    m_next = item;
    return * this;
}

GalleryItem * GalleryItem::getPrev(){
    return m_prev;
}

GalleryItem * GalleryItem::getNext(){
    return m_next;
}

void GalleryItem::rotate(float angle){
    m_angle += angle;
}

void GalleryItem::resize(float value){
	m_scale *= value;
}

void GalleryItem::move(ofVec2f force){
    m_accelerate += force;
}

ofVec2f GalleryItem::getPosition() {
    return m_position;
};

float GalleryItem::getWidth(){
    return m_size.x;
}

float GalleryItem::getHeight(){
    return m_size.y;
}

float GalleryItem::getScaledWidth(){
    return m_size.x * m_scale;
}

float GalleryItem::getScaledHeight(){
    return m_size.y * m_scale;
}

void GalleryItem::update(){
    
    ofVec2f direction;

    if (m_state == CENTER){
        if ((getScaledHeight() < m_img.getHeight()) &&
            (getScaledHeight() < m_gallery->height * m_gallery->selected_scale ) &&
            (getScaledWidth()  < m_gallery->width  * m_gallery->selected_scale ) ) {
            m_scale += 0.01;
        }
        
        direction = ofVec2f(m_gallery->x, m_gallery->y) + ofVec2f(m_gallery->width, m_gallery->height) * 0.5;
        direction -= m_position;
        
    } else if (m_state == HORIZONTAL_ALIGN){
        if ( getScaledHeight() >= m_gallery->thumb_height ) {
            m_scale -= 0.01;
        }

        if (m_back_looking && m_prev != NULL) {
            direction = m_prev->getPosition();

            direction.x += m_prev->getScaledWidth() * 0.5 + 10 + getScaledWidth() * 0.5;
            direction.y  = m_gallery->y + m_gallery->height *0.5;
            direction   -= m_position;
        } else if (m_next != NULL){
            direction = m_next->getPosition();

            direction.x -= m_next->getScaledWidth() * 0.5 + 10 + getScaledWidth() * 0.5;
            direction.y  = m_gallery->y + m_gallery->height *0.5;
            direction   -= m_position;
        
        }
    }

    move(direction);

	m_velocity += m_accelerate;
	m_velocity *= m_gallery->resistence;

	m_position += m_velocity;

	m_accelerate *= 0;
}

void GalleryItem::draw(){
	ofEnableAlphaBlending();
	
	ofPushMatrix();

	ofTranslate(m_position.x, m_position.y);
	ofScale(m_scale, m_scale, 1);
	ofRotateZ(ofRadToDeg(m_angle));
    
	ofSetColor(255);
	ofFill();
	
	m_img.draw(-m_size.x * 0.5, -m_size.y * 0.5, m_size.x, m_size.y);
    
	ofPopMatrix();
	ofDisableAlphaBlending();
}

bool GalleryItem::isOver(ofVec2f location){	
	ofVec2f dir_to_center = m_position - location;

	float theta = atan2(dir_to_center.x, dir_to_center.y) - (PI/2);
	float r = dir_to_center.length();

	float x = r * cos(theta + m_angle);
	float y = r * sin(theta + m_angle);
	
	if ((x <=  m_size.x * 0.5 * m_scale ) && (x >= -m_size.x * 0.5 * m_scale ) &&
        (y <=  m_size.y * 0.5 * m_scale ) && (y >= -m_size.y * 0.5 * m_scale ) ) {
		return true;
    } else {
		return false;
    }

}

bool GalleryItem::isOver(int x, int y){
    return isOver(ofVec2f(x, y));
}