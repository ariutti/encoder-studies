class Slice
{
  PVector pos;
  int idx;
  float r;
  float alpha;
  boolean bHighlight;
 
  Slice(int _idx, float _r, float _alpha) {
    pos = new PVector(0,0);
    idx = _idx;
    r = _r;
    alpha = _alpha;
    bHighlight = false;
  }
  
  void display() {
    pushMatrix();
    pushStyle();
    translate(pos.x, pos.y);
    rotate( idx*alpha );
    
    noStroke();
    //stroke(0);
    //strokeWeight(3);
    
    if(bHighlight)
      fill(255);
    else
      fill(255, 120);
   
    arc(pos.x, pos.y, r*2, r*2, 0, alpha, PIE);
    popStyle();
    popMatrix();
  }
  
  void select() {
    bHighlight = true;
  }
  
  void deselect() {
    bHighlight = false;
  }
}