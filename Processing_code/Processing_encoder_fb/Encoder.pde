class Encoder
{
  PVector pos;
  float r;
  int nSlices;
  float angle;
  Slice[] slices;
  int sliceIdx;

  Encoder( int _nSlices, float _r ) {
    pos = new PVector(width*0.5, height*0.5);
    r = _r;
    nSlices = _nSlices;
    angle = 2*PI/nSlices;
    slices = new Slice[ nSlices ];
    for(int i=0; i<nSlices; i++)
    {
      slices[i] = new Slice( i, r, angle);
    }
    sliceIdx = 0;
    select( sliceIdx );
  }
   
  void display() {
    pushMatrix();
    pushStyle();
    translate( pos.x, pos.y );
    
    for(int i=0; i<nSlices; i++) {
      slices[i].display();
    }
    
    stroke(0);
    fill(0);
    ellipse(0, 0, r*2*2/3.0, r*2*2/3.0);
    
    
    popStyle();
    popMatrix();
  }
  
  void select( int inc ) {
    sliceIdx += inc;
    
    if(sliceIdx < 0)
      sliceIdx += nSlices;
    else
      sliceIdx %= nSlices;
    
    for(int i=0; i<nSlices; i++) {
      slices[i].deselect();
    }
    slices[ sliceIdx ].select();
  }
  
}