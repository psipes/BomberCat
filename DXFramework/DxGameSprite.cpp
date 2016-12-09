#include "stdafx.h"
#include <cassert>
#include "DxFramework/DxCommon.h"
#include "DxFramework/DxTypes.h"
#include "DxFramework/DxGameSprite.h"

//=======================================================================
DxGameSprite::DxGameSprite ( )
:myRotation(0), isCollidable(true), myVisible(true), isDestroyable(true), myCenter( 0, 0 ), 
myVelocity( 0, 0, 0 ), myLastVelocity( 0, 0, 0 ), myAccel( 0, 0, 0 ), 
myLastAccel( 0, 0, 0 ),myScale( 1, 1 ), myPosition( 0, 0, 0 ), myLastPosition ( 0, 0, 0 )
{
   myAnimationIsValid = false;
}

//=======================================================================
DxGameSprite::~DxGameSprite( )
{
	destroy();
}

//=======================================================================
//make create that takes in width, height, and x+y positions
bool DxGameSprite::create( const tstring& animationName, float speed, D3DCOLOR transcolor )
{
	//create animation here
	myAnimation = DxAssetManager::getInstance().getAnimationCopy( animationName, speed, transcolor );

	myCollisionArea.top  = (long)myPosition.y;
	myCollisionArea.bottom = (long)myCollisionArea.top + getHeight();
	myCollisionArea.left = (long)myPosition.x;
	myCollisionArea.right = (long)myCollisionArea.left + getWidth();
	myLastCA = myCollisionArea;
   D3DCOLOR color = D3DCOLOR_XRGB( 255, 255, 255);
   myColor = color;
	myAnimation.play();
   myAnimationIsValid = true;
	return true;
}

//=======================================================================
//make create that takes in width, height, and x+y positions
bool DxGameSprite::create( DxTexture* texture, D3DCOLOR transcolor )
{
	//create animation here

	if ( !myAnimation.init( texture, 0 ) )
	{
		return false;
	}

	myCollisionArea.top  = (long)myPosition.y;
	myCollisionArea.bottom = (long)myCollisionArea.top + getHeight();
	myCollisionArea.left = (long)myPosition.x;
	myCollisionArea.right = (long)myCollisionArea.left + getWidth();
	myLastCA = myCollisionArea;
    myColor = D3DCOLOR_XRGB( 255, 255, 255 );
    myAnimationIsValid = true;
	return true;
}



//=======================================================================
void DxGameSprite::destroy ( )
{
   myAnimation.shutdown();
	myCollisionArea.set( 0, 0, 0, 0 );
	myLastCA = myCollisionArea;
   myAnimationIsValid = false;
}


//=======================================================================
bool DxGameSprite::changeAnimation( const tstring& animationName, float speed, D3DCOLOR color )
{
	myAnimation = DxAssetManager::getInstance().getAnimationCopy( animationName, speed, color );
	return true;
}

//=======================================================================
bool DxGameSprite::changeAnimation( DxAnimation& newAnimation )
{
	myAnimation = newAnimation;
	return true;
}

//=======================================================================
DxAnimation& DxGameSprite::getAnimation( )
{
	return myAnimation;
}

//=======================================================================
void DxGameSprite::transform ( float x, float y, D3DXVECTOR2 center, float rotation, D3DXVECTOR2 scale, D3DCOLOR color )
{
	this->setXPosition( x );
	this->setYPosition( y );
	myCenter = center;
	myRotation = rotation;
	myScale = scale;
	myColor = color;
}

//=======================================================================
void DxGameSprite::setPosition ( const D3DXVECTOR3& position )
{
   myPosition = position;
}

//=======================================================================
void DxGameSprite::setPosition ( float x, float y )
{
	setXPosition( x );
	setYPosition( y );
}

//=======================================================================
void DxGameSprite::setXPosition ( float value )
{
	//set XPOS
	myPosition.x = value;
	myLastCA = myCollisionArea;
	//update collision Area to new XPOS
	myCollisionArea.x( (long)myPosition.x );

}

//=======================================================================
void DxGameSprite::setYPosition ( float value )
{
	//set YPOS
	myPosition.y = value;

	myLastCA = myCollisionArea;
	//update Collision Area to new YPOS
	myCollisionArea.y( (long)myPosition.y );
}

//===========================================================================
// rotation is in radians
void DxGameSprite::setRotation( float rotation )
{
   myRotation = rotation;
}


//===========================================================================
void DxGameSprite::setScale ( float scaleX, float scaleY )
{
	myScale.x = scaleX;
	myScale.y = scaleY;
}
//===========================================================================
void DxGameSprite::draw (IDXSPRITE spriteObj, D3DCOLOR color)
{
   if ( myAnimationIsValid && isVisible() )
   {
      myAnimation.drawFrame ( spriteObj, &myPosition, &myScale, 
         myRotation, &myCenter,  color );
   }
}

void DxGameSprite::drawMyColor( IDXSPRITE spriteObj )
{
   if ( myAnimationIsValid && isVisible() )
   {
      myAnimation.drawFrame ( spriteObj, &myPosition, &myScale, 
         myRotation, &myCenter,  myColor );
   }
}

//=======================================================================

void DxGameSprite::setXVel (float xV)
{
	myVelocity.x = xV;
}

//=======================================================================

void DxGameSprite::setYVel (float yV)
{
	myVelocity.y = yV;
}

//=======================================================================

void DxGameSprite::evaluateCenter ()
{
	myCenter = D3DXVECTOR2( ( myPosition.x + ( getWidth() / 2 ) ), myPosition.y + ( ( getHeight() / 2 ) ) );
}



//=======================================================================

void DxGameSprite::setLastXVel ( float xV )
{
	myLastVelocity.x = xV;
}

//=======================================================================

void DxGameSprite::setLastYVel ( float yV )
{
	myLastVelocity.y = yV;
}

//=======================================================================

void DxGameSprite::update()
{
	myLastPosition = myPosition;

	// Velocity Updates

	// Position Updates
	myVelocity     += myAccel;
	myPosition     += myVelocity;

	// update collision area
	myCollisionArea.x( (long)myPosition.x + myCollisionOffset.x);
	myCollisionArea.y( (long)myPosition.y + myCollisionOffset.y);
   if ( myAnimationIsValid )
	   myAnimation.update();

}

//=======================================================================
bool DxGameSprite::collidesWith ( const DxGameSprite& other )
{
	return myCollisionArea.collidesWith( other.myCollisionArea );
}

//=======================================================================
bool DxGameSprite::radialCollidesWith ( const DxGameSprite& other  )
{
	D3DXVECTOR2 centerOne, centerTwo;
	double radiusOne, radiusTwo;


	//Get the radius for the first sprite using the longest dimension
	//    because wouldn't want to have it collide with you and do nothing
	//    just because the sprite is skinny or fat or short or tall
	if( getWidth() > getHeight() )
		radiusOne = getWidth() / 2;
	else
		radiusOne = getHeight() / 2;

	centerOne.x = getXPosition() + (float)radiusOne;
	centerOne.y = getYPosition() + (float)radiusOne;

	if( other.getWidth() > other.getHeight() )
		radiusTwo = other.getWidth() / 2.0;
	else
		radiusTwo = other.getHeight() / 2.0;

	centerTwo.x = other.getXPosition() + (float)radiusTwo;
	centerTwo.y = other.getYPosition() + (float)radiusTwo;

	double deltaX = centerOne.x - centerTwo.x;
	double deltaY = centerTwo.y - centerOne.y;
	double distance = sqrt((deltaX * deltaX) + (deltaY * deltaY));


	return (distance < radiusOne + radiusTwo);

	//other.getHeight() - centerTwo.y;
	//other.getWidth() - centerTwo.x;

	////Gives center but need radius
	//evaluateCenter();
	//centerOne = getCenter();

	//Gives center but need radius
	//other.evaluateCenter();
	//centerTwo = other.getCenter();


	//Calculate distance


}
//=======================================================================

void DxGameSprite::toggleVisible ()
{
	myVisible = !myVisible;
}

//=======================================================================
void DxGameSprite::setCollisionArea (RECT collisionArea)
{
	myLastCA = myCollisionArea;
	myCollisionArea = collisionArea;
	myCollisionOffset.x = myCollisionArea.left - (long)myPosition.x;
	myCollisionOffset.y =  myCollisionArea.top - (long)myPosition.y;
}

