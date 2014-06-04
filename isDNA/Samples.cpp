#include <string.h>
#include <windows.h>
#include <time.h>
#include <math.h>
#include <GL/glui.h>
#include "Dna.h"

#define GBK_FILE		"puc19.gbk"

#define CONFORMATION_1  "sample1.dat"
#define CONFORMATION_2  "sample2.dat"
#define CONFORMATION_3  "sample3.dat"
#define CONFORMATION_4  "sample4.dat"
#define CONFORMATION_5  "sample5.dat"

Dna	dna(GBK_FILE, NULL);

float xy_aspect;
int   last_x, last_y;
float rotationX = 0.0, rotationY = 0.0;

/** These are the live variables passed into GLUI ***/
int   show_skeleton = 0;
int   show_nucleotides = 1;
int   main_window;
float scale = 0.0005;
int   show_text = 1;
float view_rotate[16] = { 1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };
float obj_pos[] = { 0.0, 0.0, 0.0 };
char *string_list[] = {"Circular", "Sample 1", "Sample 2", "Sample 3", "Sample 4", "Sample 5"};
int   curr_string = 0;

/** Pointers to the windows and some of the controls we'll create **/
GLUI *glui, *glui2;

/********** Miscellaneous global variables **********/
GLfloat light0_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light0_diffuse[] =  {.5f, .5f, .5f, 1.0f};
GLfloat light0_position[] = {.5f, .5f, 1.0f, 0.0f};

GLfloat light1_ambient[] =  {0.1f, 0.1f, 0.3f, 1.0f};
GLfloat light1_diffuse[] =  {.5f, .5f, .5f, 1.0f};
GLfloat light1_position[] = {-1.0f, -1.0f, 1.0f, 0.0f};

GLfloat lights_rotation[16] = {1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1 };


/**************************************************/
double unifRand(void) {

    return rand() / double(RAND_MAX);
}


/**************************************** myGlutKeyboard() **********/

void myGlutKeyboard(unsigned char Key, int x, int y)
{
  switch(Key)
  {
  case 27: 
  case 'q':
    std::exit(0);
    break;
  };
}


/***************************************** myGlutMenu() ***********/

void myGlutMenu( int value )
{
  myGlutKeyboard( value, 0, 0 );
}


/***************************************** myGlutIdle() ***********/

void myGlutIdle( void )
{
  /* According to the GLUT specification, the current window is 
     undefined during an idle callback.  So we need to explicitly change
     it if necessary */
  if ( glutGetWindow() != main_window ) 
    glutSetWindow(main_window);  

  /*  GLUI_Master.sync_live_all();  -- not needed - nothing to sync in this
                                       application  */

  glutPostRedisplay();
}

/***************************************** myGlutMouse() **********/

void myGlutMouse(int button, int button_state, int x, int y )
{
}


/***************************************** myGlutMotion() **********/

void myGlutMotion(int x, int y )
{
  glutPostRedisplay(); 
}

/**************************************** myGlutReshape() *************/

void myGlutReshape( int x, int y )
{
  int tx, ty, tw, th;
  GLUI_Master.get_viewport_area( &tx, &ty, &tw, &th );
  glViewport( tx, ty, tw, th );

  xy_aspect = (float)tw / (float)th;

  glutPostRedisplay();
}

/**************************************** myGlutReshape() *************/

void drawOrthogonalAxis(double size) {

	glPushMatrix();

	glBegin(GL_LINES);
		glVertex3f(-2000,0,0);
		glVertex3f(2000,0,0);
		glVertex3f(0,-2000,0);
		glVertex3f(0,2000,0);
		glVertex3f(0,0,-2000);
		glVertex3f(0,0,2000);
	glEnd();

	// X arrow
	glPushMatrix();
	glTranslatef(2000,0,0);
	glRotatef(90,0,1,0);
	glutSolidCone(25,100,10,10);
	glPopMatrix();

	// Y arrow
	glPushMatrix();
	glTranslatef(0,2000,0);
	glRotatef(-90,1,0,0);
	glutSolidCone(25,100,10,10);
	glPopMatrix();

	// Z arrow
	glPushMatrix();
	glTranslatef(0,0,2000);
	glutSolidCone(25,100,10,10);
	glPopMatrix();

	glPopMatrix();
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay( void )
{

  // background color
  glClearColor( 1.0f, 1.0f, 1.0f, 1.0f );

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

  glMatrixMode( GL_PROJECTION );
  glLoadIdentity();
  glFrustum( -xy_aspect*.04, xy_aspect*.04, -.04, .04, .1, 15.0 );

  glMatrixMode( GL_MODELVIEW );

  glLoadIdentity();
  glMultMatrixf( lights_rotation );
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
  glLoadIdentity();
  glTranslatef( 0.0, 0.0, -2.6f );
  glTranslatef( obj_pos[0], obj_pos[1], -obj_pos[2] ); 
  glMultMatrixf( view_rotate );

  glScalef(scale, scale, scale);

  // x,y,z
  drawOrthogonalAxis(2000);

  switch (curr_string) {
	  case 1: 
			dna.loadConformation(CONFORMATION_1);
			break;
	  case 2: 
			dna.loadConformation(CONFORMATION_2);
			break;
	  case 3: 
			dna.loadConformation(CONFORMATION_3);
			break;
	  case 4: 
			dna.loadConformation(CONFORMATION_4);
			break;
	  case 5: 
			dna.loadConformation(CONFORMATION_5);
			break;
	  default: 
			dna.circularConformation();
			break;
  }

  // render walk
  if (show_skeleton) {
	  dna.drawSkeleton();
  }

  // render building blocks
  if (show_nucleotides) {
	  dna.buildAxis();
	  dna.draw();
  }

  if (show_text) 
  {
    glDisable( GL_LIGHTING );  /* Disable lighting while we render text */
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    gluOrtho2D( 0.0, 100.0, 0.0, 100.0  );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    glColor3ub( 0, 0, 0 );
    glRasterPos2i( 1, 1 );
    /*** Render the live character array 'text' ***/
    int i;
    for( i=0; i<(int)strlen( string_list[curr_string] ); i++ )
      glutBitmapCharacter( GLUT_BITMAP_HELVETICA_18, string_list[curr_string][i] );
  }

  glEnable(GL_LIGHTING);

  glutSwapBuffers(); 
}

/**************************************** control_cb() *******************/
/* GLUI control callback                                                 */

void control_cb( int control ) {

	// To Do
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
  /****************************************/
  /*   Initialize GLUT and create window  */
  /****************************************/

  glutInit(&argc, argv);
  glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowPosition( 50, 50 );
  glutInitWindowSize( 800, 600 );
 
  main_window = glutCreateWindow("isDNA");
  glutDisplayFunc( myGlutDisplay );
  GLUI_Master.set_glutReshapeFunc( myGlutReshape );  
  GLUI_Master.set_glutKeyboardFunc( myGlutKeyboard );
  GLUI_Master.set_glutSpecialFunc( NULL );
  GLUI_Master.set_glutMouseFunc( myGlutMouse );
  glutMotionFunc( myGlutMotion );

  srand(time(NULL));

  /****************************************/
  /*       Set up OpenGL lights           */
  /****************************************/

  glEnable(GL_LIGHTING);
  glEnable( GL_NORMALIZE );

  glEnable(GL_LIGHT0);
  glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);

  glEnable(GL_LIGHT1);
  glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

  /****************************************/
  /*          Enable z-buferring          */
  /****************************************/

  glEnable(GL_DEPTH_TEST);

  /****************************************/
  /*         Here's the GLUI code         */
  /****************************************/

  /*** Create the side subwindow ***/
  glui = GLUI_Master.create_glui_subwindow( main_window, 
					    GLUI_SUBWINDOW_RIGHT );

  /**** Add listbox ****/
  new GLUI_StaticText( glui, "" );
  GLUI_Listbox *sequences = new GLUI_Listbox( glui, "Sequence", NULL );
  sequences->add_item( 0, "pUC19");

  /**** Add listbox ****/
  new GLUI_StaticText( glui, "" );
  GLUI_Listbox *list = new GLUI_Listbox( glui, "Conformation", &curr_string );
  int i;
  for( i=0; i<=5; i++ )
    list->add_item( i, string_list[i] );

  new GLUI_StaticText( glui, "" );

  /***** Control for object params *****/
  new GLUI_Checkbox( glui, "Skeleton", &show_skeleton, 1, control_cb );
  new GLUI_Checkbox( glui, "Nucleotides", &show_nucleotides, 1, control_cb );

  new GLUI_StaticText( glui, "" );
  /****** A 'quit' button *****/
  new GLUI_Button( glui, "Quit", 0,(GLUI_Update_CB)exit );

  /**** Link windows to GLUI, and register idle callback ******/
  glui->set_main_gfx_window( main_window );


  /*** Create the bottom subwindow ***/
  glui2 = GLUI_Master.create_glui_subwindow( main_window, 
                                             GLUI_SUBWINDOW_BOTTOM );
  glui2->set_main_gfx_window( main_window );

  GLUI_Rotation *view_rot = new GLUI_Rotation(glui2, "Rotate", view_rotate );
  view_rot->set_spin( 1.0 );
  new GLUI_Column( glui2, false );
  GLUI_Translation *trans_xy = 
    new GLUI_Translation(glui2, "Translate XY", GLUI_TRANSLATION_XY, obj_pos );
  trans_xy->set_speed( .005 );
  new GLUI_Column( glui2, false );
  GLUI_Translation *trans_z = 
    new GLUI_Translation( glui2, "Translate Z", GLUI_TRANSLATION_Z, &obj_pos[2] );
  trans_z->set_speed( .005 );

#if 0
  /**** We register the idle callback with GLUI, *not* with GLUT ****/
  GLUI_Master.set_glutIdleFunc( myGlutIdle );
#endif

  /**** Regular GLUT main loop ****/
  
  glutMainLoop();

  return EXIT_SUCCESS;
}

