
#include "DepthCursor.h" 
	
void DepthCursor::setup ( IisuServer * _iisu , int _cursorID , ofColor _color ) 
{
	iisu = _iisu ; 
	color = _color ; 
	cursorID = _cursorID ; 
	cursorStatus = 0 ; 
	position = ofVec2f () ; 
	
	ofAddListener( IisuEvents::Instance()->CURSOR_DETECTED , this , &DepthCursor::cursorFoundHandler ) ; 
	ofAddListener( IisuEvents::Instance()->CURSOR_LOST , this , &DepthCursor::cursorLostHandler ) ; 
	ofAddListener( IisuEvents::Instance()->CURSOR_DESTROYED , this , &DepthCursor::cursorDestroyedHandler ) ;
}

void DepthCursor::update( ) 
{
	//update IISU
	if ( iisu->m_device==NULL || iisu->m_iisuHandle==NULL )
	{
		//IISU is broken !
		cout << "IISU IS BROKEN !" << endl ; 
		return ;
	}		

	cursorStatus = iisu->getCursorStatus( cursorID ) ; 

	//Calculate the palm position
	ofVec3f desiredLoc = iisu->iisuPointToOF( iisu->getNormalizedCursorCoordinates( cursorID ) ) ; 
	
	//Limit positions so that the cursor never goes off of the edge
	float padding = 50 ; 
	float maxX = ofGetWidth() - padding ; 
	float maxY = ofGetHeight() - padding ; 

	if ( desiredLoc.x < padding ) 
		desiredLoc.x = padding ; 
	if ( desiredLoc.x > maxX )
		desiredLoc.x = maxX ; 

	if ( desiredLoc.y < padding ) 
		desiredLoc.y = padding ; 
	if ( desiredLoc.y > maxY ) 
		desiredLoc.y = maxY ; 

	//A little interpolation goes a long way to make it look smoother
	Tweenzor::add( &position.x , position.x , desiredLoc.x , 0.0f , 0.12f , EASE_OUT_QUAD ) ; 
	Tweenzor::add( &position.y , position.y , desiredLoc.y , 0.0f , 0.12f , EASE_OUT_QUAD ) ; 
}

void DepthCursor::draw ( ) 
{
	ofPushStyle() ; 
	ofPushMatrix() ; 
		ofTranslate( position.x , position.y ) ; 
			ofSetColor ( color.r , color.g , color.b ) ;
			if ( cursorStatus == 0 ) 
				ofNoFill() ; 
			ofCircle( 0 , 0, 15 ) ;
	ofPopMatrix() ; 
	ofPopStyle() ;
}

void DepthCursor::debugDraw( ) 
{
	ofSetColor( 255 , 255 , 255 ) ;
	ofDrawBitmapString( "ID: " + ofToString( cursorID ) + " @status:"+ofToString( cursorStatus)  , position.x , position.y + 15 ) ; 
 	ofDrawBitmapString( "X:" + ofToString( position.x  ) + "\nY: " + ofToString( position.y ) , position.x , position.y + 35 ) ; 
}



void DepthCursor::cursorFoundHandler ( int &args ) 
{
	bActive = true ; 
	cout << "cursor " << cursorID << " found ! " << endl ;
}

void DepthCursor::cursorLostHandler ( int &args ) 
{
	cout << "cursor " << cursorID << " lost ! " << endl ;
}

void DepthCursor::cursorDestroyedHandler ( int &args ) 
{
	cout << "cursor " << cursorID << " destroyed ! " << endl ;
}
