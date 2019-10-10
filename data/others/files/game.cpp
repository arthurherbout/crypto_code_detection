/*
Copyright (c) 2012, Lunar Workshop, Inc.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
3. All advertising materials mentioning features or use of this software must display the following acknowledgement:
   This product includes software developed by Lunar Workshop, Inc.
4. Neither the name of the Lunar Workshop nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY LUNAR WORKSHOP INC ''AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL LUNAR WORKSHOP BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "game.h"

#include <cstring>

#include <algorithm>

#include <GL3/gl3w.h>

#if defined(__APPLE__)
#include <OpenGL/glu.h>
#include <unistd.h>
#else
#include <GL/glu.h>
#endif

#include <mtrand.h>
#include <math/collision.h>
#include <math/frustum.h>
#include <maths.h>
#include <math/quaternion.h>
#include <math/physics.h>

#include <renderer/renderer.h>
#include <renderer/renderingcontext.h>
#include <renderer/shaders.h>

#include "character.h"

#include "seaweed.h"

#include "spline.h"

#define SEGMENTS_PER_POINT 60

Vector g_spline_points[SPLINE_POINTS] =
{
	Vector(0, 1, 0),
	Vector(5, 1, 0),
	Vector(10, 1, 10),
	Vector(40, 1, 15),
	Vector(15, 1, -10),
	Vector(-50, 1, 30),
	Vector(-40, 1, 19),
	Vector(-20, 1, -5),
	Vector(8, 1, 17),
	Vector(9, 1, 16),
	Vector(10, 1, 15),
	Vector(11, 1, 14),
	Vector(12, 1, 13),
	Vector(13, 1, 12),
	Vector(14, 1, 11),
	Vector(15, 1, -15),
};

Vector g_spline_segments[(SPLINE_POINTS+1) * SEGMENTS_PER_POINT];
float g_spline_speed = 1.5f;

CGame::CGame(int argc, char** argv)
	: CApplication(argc, argv)
{
	m_hPlayer = nullptr;

	m_iLastMouseX = m_iLastMouseY = -1;

	memset(m_apEntityList, 0, sizeof(m_apEntityList));
}

void CGame::Load()
{
	m_iMonsterTexture = GetRenderer()->LoadTextureIntoGL("monster.png");
	m_iCrateTexture = GetRenderer()->LoadTextureIntoGL("crate.png");
	m_iNormalTexture = GetRenderer()->LoadTextureIntoGL("normal.png");

	GraphReset();

	m_projectile_initial_time = Game()->GetTime();
	m_projectile_initial_position = Vector(2, 1, 2);
	m_projectile_initial_velocity = Vector(-1, 3, -1) * 5;
	m_projectile_gravity = Vector(0, -5, 0);
	m_projectile_break_time = Game()->GetTime() + PredictProjectileMaximumHeightTime(m_projectile_initial_velocity, m_projectile_gravity);
	m_projectile_number = 1;

	// Fire the first one
	m_projectile_position[0] = m_projectile_initial_position;
	m_projectile_velocity[0] = m_projectile_initial_velocity;

#if DO_SEAWEED
	for (int k = 0; k < sizeof(g_seaweed) / sizeof(g_seaweed[0]); k++)
	{
		g_seaweed[k].m_positions[0][0] = g_seaweed[k].m_positions[1][0] = vec3(g_seaweed_positions[k].x, 0, g_seaweed_positions[k].y);
		for (int n = 1; n < SEAWEED_LINKS; n++)
			g_seaweed[k].m_positions[0][n] = g_seaweed[k].m_positions[1][n] = g_seaweed[k].m_positions[0][n - 1] + vec3(0, g_seaweed_link_length, 0);
	}
#endif

	memcpy(g_spline.m_points, g_spline_points, sizeof(g_spline_points));
	g_spline.InitializeSpline();

	float total_length = g_spline.GetTotalLength();

	for (int k = 0; k < VArraySize(g_spline_segments); k++)
		g_spline_segments[k] = g_spline.ConstVelocitySplineAtTime(total_length*k/VArraySize(g_spline_segments)/g_spline_speed, g_spline_speed);

	int num_particles = VArraySize(m_particles);
	int num_satellites = VArraySize(m_satellites);
	m_particle_paths.resize(num_satellites + num_particles);
	int path_index = 0;

	for (int k = 0; k < num_particles; k++)
	{
		m_particles[k] = Vector(-2, 2, 1.0f/3);
		m_particle_paths[path_index++].push_back(Vector(m_particles[k].x*m_particle_scale, 2, m_particles[k].z*m_particle_scale));
	}

	for (int k = 0; k < num_satellites; k++)
	{
		m_satellites[k].m_position = m_satellite_x0;
		m_satellites[k].m_velocity = m_satellite_v0;
		m_satellites[k].m_last_position = m_satellites[k].m_position - m_satellites[k].m_velocity * m_satellite_timestep;
		m_particle_paths[path_index++].push_back(Vector(m_satellites[k].m_position.x, 2, m_satellites[k].m_position.z));
	}

	m_stars[0] = Vector(0, 2, 0);
	m_stars[1] = Vector(-0.5, 2, -0.5);
	m_stars[2] = Vector(0, 2, -0.5);
	m_stars[3] = Vector(-0.5, 2, 0);
	m_stars[4] = Vector(-0.25, 2, -0.25);
}

void CGame::MakePuff(const Point& p)
{
	m_aPuffs.push_back(CPuff());

	m_aPuffs.back().vecOrigin = p;
	m_aPuffs.back().flTimeCreated = GetTime();
}

void CGame::MakeBulletTracer(const Point& s, const Point& e)
{
	m_aTracers.push_back(CBulletTracer());

	m_aTracers.back().vecStart = s;
	m_aTracers.back().vecEnd = e;
	m_aTracers.back().flTimeCreated = GetTime();
}

// This method gets called when the user presses a key
bool CGame::KeyPress(int c)
{
	if (c == 'W')
	{
		m_hPlayer->m_vecMovementGoal.x = m_hPlayer->m_flSpeed;
		return true;
	}
	else if (c == 'A')
	{
		m_hPlayer->m_vecMovementGoal.z = m_hPlayer->m_flSpeed;
		return true;
	}
	else if (c == 'S')
	{
		m_hPlayer->m_vecMovementGoal.x = -m_hPlayer->m_flSpeed;
		return true;
	}
	else if (c == 'D')
	{
		m_hPlayer->m_vecMovementGoal.z = -m_hPlayer->m_flSpeed;
		return true;
	}
	else if (c == ' ')
	{
		m_hPlayer->m_vecVelocity.y = 7;
		return true;
	}
	else if (c == 'G')
	{
		GraphStep();
		return true;
	}
	else if (c == 'H')
	{
		GraphComplete();
		return true;
	}
	else if (c == 'R')
	{
		GraphReset();
		return true;
	}
	else
		return CApplication::KeyPress(c);
}

// This method gets called when the player releases a key.
void CGame::KeyRelease(int c)
{
	if (c == 'W')
	{
		m_hPlayer->m_vecMovementGoal.x = 0;
	}
	else if (c == 'A')
	{
		m_hPlayer->m_vecMovementGoal.z = 0;
	}
	else if (c == 'S')
	{
		m_hPlayer->m_vecMovementGoal.x = 0;
	}
	else if (c == 'D')
	{
		m_hPlayer->m_vecMovementGoal.z = 0;
	}
	else
		CApplication::KeyPress(c);
}

// This method is called every time the player moves the mouse
void CGame::MouseMotion(int x, int y)
{
	if (!HasFocus())
	{
		// Swallow the input while the window isn't in focus so the player
		// isn't facing off in a strange direction when they tab back in.
		m_iLastMouseX = x;
		m_iLastMouseY = y;
		return;
	}

	if (m_iLastMouseX == -1 && m_iLastMouseY == -1)
	{
		m_iLastMouseX = x;
		m_iLastMouseY = y;
	}

	int iMouseMovedX = x - m_iLastMouseX;
	int iMouseMovedY = m_iLastMouseY - y; // The data comes in backwards. negative y means the mouse moved up.

	if (!m_hPlayer)
		return;

	float flSensitivity = 0.3f;

	EAngle angView = m_hPlayer->GetLocalView();

	angView.p += iMouseMovedY*flSensitivity;
	angView.y += iMouseMovedX*flSensitivity;
	angView.Normalize();

	m_hPlayer->SetLocalView(angView);

	m_iLastMouseX = x;
	m_iLastMouseY = y;
}

bool CGame::MouseInput(int iButton, tinker_mouse_state_t iState)
{
	if (iButton == TINKER_KEY_MOUSE_LEFT && iState == TINKER_MOUSE_PRESSED)
	{
		Vector v0 = m_hPlayer->GetGlobalOrigin() + Vector(0, 1, 0);
		Vector v1 = m_hPlayer->GetGlobalOrigin() + Vector(0, 1, 0) + m_hPlayer->GetGlobalView() * 100;

		Vector vecIntersection;
		CCharacter* pHit = nullptr;
		if (TraceLine(v0, v1, vecIntersection, pHit))
		{
			MakePuff(vecIntersection);
			MakeBulletTracer(v0, vecIntersection);

			if (pHit)
			{
				pHit->m_flShotTime = Game()->GetTime();
				pHit->TakeDamage(1);
			}
		}
		else
			MakeBulletTracer(v0, v1);

		return true;
	}

	if (iButton == TINKER_KEY_MOUSE_RIGHT && iState == TINKER_MOUSE_PRESSED)
		sleep(1);

	return false;
}

// Trace a line through the world to simulate, eg, a bullet http://www.youtube.com/watch?v=USjbg5QXk3g
bool CGame::TraceLine(const Vector& v0, const Vector& v1, Vector& vecIntersection, CCharacter*& pHit)
{
	float flLowestFraction = 1;

	Vector vecTestIntersection;
	float flTestFraction;
	pHit = nullptr;

	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		CCharacter* pCharacter = GetCharacterIndex(i);
		if (!pCharacter)
			continue;

		// Only monsters and boxes get hit by traces. The player doesn't, he's immune to his own attacks.
		if (!pCharacter->m_bHitByTraces)
			continue;

		Matrix4x4 mInverse = pCharacter->GetGlobalTransform().InvertedTR();

		Matrix4x4 scale;
		scale.SetScale(1/pCharacter->m_vecScaling);

		mInverse = scale*mInverse;

		// The v0 and v1 are in the global coordinate system and we need to transform it to the target's
		// local coordinate system to use axis-aligned intersection. We do so using the inverse transform matrix.
		// http://youtu.be/-Fn4atv2NsQ
		if (LineAABBIntersection(pCharacter->m_aabbSize, mInverse*v0, mInverse*v1, vecTestIntersection, flTestFraction) && flTestFraction < flLowestFraction)
		{
			// Once we have the result we can use the regular transform matrix to get it back in
			// global coordinates. http://youtu.be/-Fn4atv2NsQ
			vecIntersection = pCharacter->GetGlobalTransform()*vecTestIntersection;
			flLowestFraction = flTestFraction;
			pHit = pCharacter;
		}
	}

	// Intersect with the floor.
	// Line-Plane Intersection algorithm: http://youtu.be/fIu_8b2n8ZM
	if (LinePlaneIntersection(Vector(0, 1, 0), Vector(0, 0, 0), v0, v1, vecTestIntersection, flTestFraction) && flTestFraction < flLowestFraction)
	{
		vecIntersection = vecTestIntersection;
		flLowestFraction = flTestFraction;
		pHit = nullptr;
	}

	if (flLowestFraction < 1)
		return true;

	return false;
}

static Vector VelocityField(Vector position, float time)
{
	return Vector(-(position.x+1)*(position.x+3), 0, -5*position.z+5*time*time+2*time);
}

// In this Update() function we need to update all of our characters. Move them around or whatever we want to do.
// http://www.youtube.com/watch?v=c4b9lCfSDQM
void CGame::Update(float dt)
{
	Vector x0 = m_hPlayer->GetGlobalOrigin();

	// The approach function http://www.youtube.com/watch?v=qJq7I2DLGzI
	m_hPlayer->m_vecMovement.x = Approach(m_hPlayer->m_vecMovementGoal.x, m_hPlayer->m_vecMovement.x, dt * 65);
	m_hPlayer->m_vecMovement.z = Approach(m_hPlayer->m_vecMovementGoal.z, m_hPlayer->m_vecMovement.z, dt * 65);

	Vector vecForward = m_hPlayer->GetGlobalView();
	vecForward.y = 0;
	vecForward.Normalize();

	Vector vecUp(0, 1, 0);

	// Cross product http://www.youtube.com/watch?v=FT7MShdqK6w
	Vector vecRight = vecUp.Cross(vecForward);

	float flSaveY = m_hPlayer->m_vecVelocity.y;
	m_hPlayer->m_vecVelocity = vecForward * m_hPlayer->m_vecMovement.x + vecRight * m_hPlayer->m_vecMovement.z;
	m_hPlayer->m_vecVelocity.y = flSaveY;

	// Update position and vecMovement. http://www.youtube.com/watch?v=c4b9lCfSDQM
	m_hPlayer->m_vecVelocity = m_hPlayer->m_vecVelocity + m_hPlayer->m_vecGravity * dt;
	m_hPlayer->SetTranslation(m_hPlayer->GetGlobalOrigin() + m_hPlayer->m_vecVelocity * dt);
	//m_hPlayer->SetTranslation(g_spline.SplineAtTime(fmod(Application()->GetTime()/2, SPLINE_POINTS-1)));

	// Make sure the player doesn't fall through the floor. The y dimension is up/down, and the floor is at 0.
	Vector vecTranslation = m_hPlayer->GetGlobalOrigin();
	if (vecTranslation.y < 0)
		m_hPlayer->SetTranslation(Vector(vecTranslation.x, 0, vecTranslation.z));

	// Grab the player's translation and make a translation only matrix. http://www.youtube.com/watch?v=iCazI3nKBf0
	Vector vecPosition = m_hPlayer->GetGlobalOrigin();
	Matrix4x4 mPlayerTranslation;
	mPlayerTranslation.SetTranslation(vecPosition);

	// Create a set of basis vectors that do what we need.
	vecForward = m_hPlayer->GetGlobalView();
	vecForward.y = 0;       // Flatten the angles so that the box doesn't rotate up and down as the player does.
	vecForward.Normalize(); // Re-normalize, we need all of our basis vectors to be normal vectors (unit-length)
	vecUp = Vector(0, 1, 0);  // The global up vector
	vecRight = -vecUp.Cross(vecForward).Normalized(); // Cross-product: https://www.youtube.com/watch?v=FT7MShdqK6w

	// Use these basis vectors to make a matrix that will transform the player-box the way we want it.
	// http://youtu.be/8sqv11x10lc
	Matrix4x4 mPlayerRotation(vecForward, vecUp, vecRight);

	Matrix4x4 mPlayerScaling = Matrix4x4();

	// Produce a transformation matrix from our three TRS matrices.
	// Order matters! http://youtu.be/7pe1xYzFCvA
	m_hPlayer->SetGlobalTransform(mPlayerTranslation * mPlayerRotation * mPlayerScaling);

	Vector x1 = m_hPlayer->GetGlobalOrigin();
	float flPlayerDistanceTraveled = m_hPlayer->m_flDistanceTraveled;

	// Add the distance traveled this frame.
	flPlayerDistanceTraveled += (x1 - x0).Length();

	m_hPlayer->m_flDistanceTraveled = flPlayerDistanceTraveled;

	float flMonsterSpeed = 0.5f;
	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		CCharacter* pCharacter = GetCharacterIndex(i);
		if (!pCharacter)
			continue;

		if (!pCharacter->m_bEnemyAI)
			continue;

		// Update position and movement. http://www.youtube.com/watch?v=c4b9lCfSDQM
		pCharacter->m_vecVelocity = (m_hPlayer->GetGlobalOrigin() - pCharacter->GetGlobalOrigin()).Normalized() * flMonsterSpeed;

		pCharacter->SetTranslation(pCharacter->GetGlobalOrigin() + pCharacter->m_vecVelocity * dt);

		if (pCharacter->m_flShotTime >= 0)
		{
			float lerp = RemapClamp(Game()->GetTime(),
				pCharacter->m_flShotTime, pCharacter->m_flShotTime + 2,
				0, 1);

			lerp = CubicInterpolation(lerp);

			float size = RemapClamp(lerp, 0, 1, 2, 5);

			pCharacter->m_vecScaling = Vector(size, size, size);
		}
	}

/*
	if (Game()->GetTime() >= m_projectile_initial_time + 8)
	{
		m_projectile_position[0] = m_projectile_initial_position;
		m_projectile_velocity[0] = m_projectile_initial_velocity = Vector((float)(mtrand()%1000)/250-2, 2.5, (float)(mtrand()%1000)/250-2) * 5;
		m_projectile_initial_time = Game()->GetTime();
		m_projectile_break_time = Game()->GetTime() + PredictProjectileMaximumHeightTime(m_projectile_initial_velocity, m_projectile_gravity);
		m_projectile_number = 1;
	}

	if (Game()->GetTime() >= m_projectile_break_time && m_projectile_number == 1)
	{
		for (int i = 1; i < MAX_PROJECTILES; i++)
		{
			m_projectile_position[i] = m_projectile_position[0];
			m_projectile_velocity[i] = m_projectile_velocity[0] + Vector((float)(mtrand()%1000)/250-2, (float)(mtrand()%1000)/250-2, (float)(mtrand()%1000)/250-2);
		}
		m_projectile_number = MAX_PROJECTILES;
	}

	// Simulate the projectile
	for (int i = 0; i < m_projectile_number; i++)
	{
		m_projectile_position[i] = m_projectile_position[i] + m_projectile_velocity[i] * dt;
		m_projectile_velocity[i] = m_projectile_velocity[i] + m_projectile_gravity * dt;

		if (m_projectile_position[i].y < 0)
		{
			MakePuff(m_projectile_position[i]);
			m_projectile_position[i].y = 9999999; // Move it way up high and out of sight until it gets reset. Sort of a hack, no big deal.
		}
	}
	*/

	while (g_seaweed_simulation_time < Game()->GetTime())
		SimulateSeaweed();

	int path_index = 0;

	// Velocity field
	{
		float t = Game()->GetTime();
		float h = m_particle_timestep;
		for (; m_particle_time < Game()->GetTime(); m_particle_time += h)
		{
			for (int k = 0; k < VArraySize(m_particles); k++)
			{
#define FIELD_EULER    1
#define FIELD_MIDPOINT 1
#define FIELD_RK4      1

				Vector xn = m_particles[k];
				float t = m_particle_time;

				switch (k)
				{
				default: {
				} break;
#if FIELD_EULER
				case 0: {
					// Standard Euler
					m_particles[k] = xn + h * VelocityField(xn, t);
				} break;
#endif

#if FIELD_MIDPOINT
				case 1: {
					// Midpoint
					Vector midpoint_position = xn + (h/2) * VelocityField(xn, t);
					m_particles[k] = xn + VelocityField(midpoint_position, t + h/2) * h;
				} break;
#endif

#if FIELD_RK4
				case 2: {
					// RK4
					Vector k1 = VelocityField(xn, t);
					Vector k2 = VelocityField(xn + h/2 * k1, t + h/2);
					Vector k3 = VelocityField(xn + h/2 * k2, t + h/2);
					Vector k4 = VelocityField(xn + h * k3, t + h);
					m_particles[k] = xn + h/6 * (k1 + 2*k2 + 2*k3 + k4);
				} break;
#endif
				}

				m_particle_paths[k].push_back(Vector(m_particles[k].x*m_particle_scale, 2, m_particles[k].z*m_particle_scale));
			}
		}
	}

	path_index = VArraySize(m_particles);

	// Satellites
	[this, &path_index](){
	{
		float G = 1; // This is not the real gravitational constant, it's a much larger one for our test system.
		float h = m_satellite_timestep;
		float mass_satellite = 1; // This doesn't really matter for our gravity calculations, as Galileo showed
		float mass_sun = 10.0f;
		float t = Game()->GetTime();
		Vector player_position = m_hPlayer->GetGlobalOrigin();

		auto force_acceleration = [this, G, mass_sun, t, player_position](Vector position, Vector velocity) -> Vector
		{
			Vector avg(0,0,0);

			Vector gravity(0,0,0);
			for (int k = 0; k < VArraySize(m_stars); k++)
			{
				Vector to_center = (m_stars[k] - position);
				float r = to_center.Length();
				gravity = gravity + to_center * ((G * mass_sun) / (r*r*r));

				avg = avg + m_stars[k];
			}

			avg = avg / VArraySize(m_stars);
			Vector to_center = (avg - position);

			Vector drag;// = -velocity.Normalized() * (velocity.LengthSqr() / (float)(1<<6));

			// Boost around the sun if you get too close.
			float boost_around_sun_strength = std::min(1/to_center.Length() / 10, 10.0f);
			Vector boosters;// = velocity * boost_around_sun_strength;

			// Boost towards the sun if you get too far.
			float boost_toward_sun_strength = std::min(to_center.Length() / 30, 10.0f);
			//boosters = boosters + to_center.Normalized() * boost_toward_sun_strength;

			// Boost away from the player
			Vector to_player = (player_position-position);
			to_player.y = 0;
			//boosters = boosters - to_player.Normalized() * 2 / to_player.Length();

			return gravity + drag + boosters;
		};

		for (; m_satellite_time < Game()->GetTime(); m_satellite_time += h)
		{
			float t = m_satellite_time;
			for (int k = 0; k < VArraySize(m_satellites); k++)
			{
				Vector xn = m_satellites[k].m_position;
				Vector vn = m_satellites[k].m_velocity;

				auto v = [vn, force_acceleration](Vector x, float h) -> Vector
				{
					return vn + force_acceleration(x, vn) * h;
				};

				auto a = [xn, force_acceleration](Vector v, float h) -> Vector
				{
					return force_acceleration(xn, v) + force_acceleration(xn + v*h, v) * h;
				};

#define PHYS_EULER    1
#define PHYS_SI_EULER 1
#define PHYS_VERLET   1
#define PHYS_MIDPOINT 1
#define PHYS_RK4      1

				switch (k)
				{
				default: {
				} break;
#if PHYS_EULER
				case 0: {
					// Standard Euler
					m_satellites[k].m_position = xn + h * v(xn, 0);
					m_satellites[k].m_velocity = vn + h * a(vn, 0);
				} break;
#endif

#if PHYS_SI_EULER
				case 1: {
					// Semi-Implicit Euler
					m_satellites[k].m_position = xn + h * v(xn, h);
					m_satellites[k].m_velocity = vn + h * a(vn, 0);
				} break;
#endif

#if PHYS_VERLET
				case 2: {
					// Verlet
					Vector position = m_satellites[k].m_position;

					m_satellites[k].m_position = 2 * m_satellites[k].m_position - m_satellites[k].m_last_position + a(vn, 0) * (h*h);
					m_satellites[k].m_last_position = position;

					// Not necessary, just for rendering the velocity arrow in our demo
					m_satellites[k].m_velocity = (m_satellites[k].m_position - m_satellites[k].m_last_position) / h;
				} break;
#endif

#if PHYS_MIDPOINT
				case 3: {
					// Midpoint
					Vector midpoint_velocity = vn + (h/2) * a(vn, 0);
					m_satellites[k].m_velocity = vn + a(midpoint_velocity, h/2) * h;

					Vector midpoint_position = xn + (h/2) * v(xn, 0);
					m_satellites[k].m_position = xn + v(midpoint_position, h/2) * h;
				} break;
#endif

#if PHYS_RK4
				case 4: {
					// RK4
					{
						Vector k1 = a(vn, 0);
						Vector k2 = a(vn + h/2 * k1, h/2);
						Vector k3 = a(vn + h/2 * k2, h/2);
						Vector k4 = a(vn + h * k3, h);
						m_satellites[k].m_velocity = vn + h/6 * (k1 + 2*k2 + 2*k3 + k4);
					}

					{
						Vector k1 = v(xn, 0);
						Vector k2 = v(xn + h/2 * k1, h/2);
						Vector k3 = v(xn + h/2 * k2, h/2);
						Vector k4 = v(xn + h * k3, h);
						m_satellites[k].m_position = xn + h/6 * (k1 + 2*k2 + 2*k3 + k4);
					}
				} break;
#endif
				}

				m_particle_paths[path_index + k].push_back(Vector(m_satellites[k].m_position.x, 2, m_satellites[k].m_position.z));
			}
		}

		for (int k = 0; k < VArraySize(m_satellites); k++)
		{
			Vector xn = m_satellites[k].m_position;
			Vector vn = m_satellites[k].m_velocity;

			float potential_energy = 0;
			for (int s = 0; s < VArraySize(m_stars); s++)
				potential_energy += -G * mass_sun * mass_satellite / ((xn - m_stars[s]).Length());

			float kinetic_energy = 0.5f * mass_satellite * vn.LengthSqr();

			m_satellites[k].m_radius = (potential_energy + kinetic_energy) / 10 + 0.3f;
		}
	}
	}();
}

void CGame::Draw()
{
	Vector vecForward = m_hPlayer->GetGlobalView();
	Vector vecUp(0, 1, 0);

	// Cross-product http://www.youtube.com/watch?v=FT7MShdqK6w
	Vector vecRight = vecUp.Cross(vecForward).Normalized();

	CRenderer* pRenderer = GetRenderer();

	// Tell the renderer how to set up the camera.
	pRenderer->SetCameraPosition(m_hPlayer->GetGlobalOrigin() - vecForward * 3 + vecUp * 3 - vecRight * 0.5f);
	pRenderer->SetCameraDirection(vecForward);
	pRenderer->SetCameraUp(Vector(0, 1, 0));
	pRenderer->SetCameraFOV(90);
	pRenderer->SetCameraNear(0.1f);
	pRenderer->SetCameraFar(1000);

	// This rendering context is a tool for rendering things to the screen.
	// All of our drawing commands are part of it.
	CRenderingContext r(pRenderer);

	// Clear the depth buffer and set a background color.
	r.ClearDepth();
	r.ClearColor(Color(210, 230, 255));

	// CRenderer::StartRendering() - This function sets up OpenGL with the
	// camera information that we passed it before.
	pRenderer->StartRendering(&r);

	m_oFrameFrustum = CFrustum(r.GetProjection() * r.GetView());

	// First tell OpenGL what "shader" or "program" to use.
	r.UseProgram("model");

	// Set the sunlight direction. The y component is -1 so the light is pointing down.
	Vector vecSunlight = Vector(1, -1, 1).Normalized();

	// Uncomment this code to make the sunlight rotate:
	//Vector vecSunlight = Vector(cos(Game()->GetTime()), -1, sin(Game()->GetTime())).Normalized();

	r.SetUniform("vecSunlight", vecSunlight);

	r.SetUniform("bLighted", false);
	r.SetUniform("bDiffuse", false);

	// Render the ground.
	r.SetUniform("vecColor", Vector4D(0.6f, 0.7f, 0.9f, 1));
	r.SetUniform("vecCameraPosition", GetRenderer()->GetCameraPosition());

	r.BeginRenderTriFan();
		r.Normal(Vector(0, 1, 0));
		r.Tangent(Vector(1, 0, 0));
		r.Bitangent(Vector(0, 0, 1));
		r.TexCoord(Vector2D(0, 1));
		r.Vertex(Vector(-30, 0, -30));
		r.TexCoord(Vector2D(0, 0));
		r.Vertex(Vector(-30, 0, 30));
		r.TexCoord(Vector2D(1, 0));
		r.Vertex(Vector(30, 0, 30));
		r.TexCoord(Vector2D(1, 1));
		r.Vertex(Vector(30, 0, -30));
	r.EndRender();

	r.SetUniform("bLighted", true);

	// Prepare a list of entities to render.
	m_apRenderOpaqueList.clear();
	m_apRenderTransparentList.clear();

	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		CCharacter* pCharacter = GetCharacterIndex(i);
		if (!pCharacter)
			continue;

		// We need to scale the AABB using the character's scale values before we can use it to calculate our center/radius.
		AABB aabbSizeWithScaling = pCharacter->m_aabbSize * pCharacter->m_vecScaling;
		Vector vecCharacterCenter = pCharacter->GetGlobalOrigin() + aabbSizeWithScaling.GetCenter();
		float flCharacterRadius = aabbSizeWithScaling.GetRadius();

		// If the entity is outside the viewing frustum then the player can't see it - don't draw it.
		// http://youtu.be/4p-E_31XOPM
		if (!m_oFrameFrustum.SphereIntersection(vecCharacterCenter, flCharacterRadius))
			continue;

		if (pCharacter->m_bDrawTransparent)
			m_apRenderTransparentList.push_back(pCharacter);
		else
			m_apRenderOpaqueList.push_back(pCharacter);
	}

	// Draw all opaque characters first.
	DrawCharacters(m_apRenderOpaqueList, false);

	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		CCharacter* pCharacter = GetCharacterIndex(i);
		if (!pCharacter)
			continue;

		if (!pCharacter->m_bEnemyAI)
			continue;

		float flRadius = 3.5f;

		Vector vecIndicatorOrigin = NearestPointOnSphere(m_hPlayer->GetGlobalOrigin(), flRadius, pCharacter->GetGlobalOrigin());

		float flBoxSize = 0.1f;

		r.SetUniform("vecColor", Color(255, 0, 0, 255));
		r.RenderBox(vecIndicatorOrigin - Vector(1, 1, 1)*flBoxSize, vecIndicatorOrigin + Vector(1, 1, 1)*flBoxSize);
	}

	// Sort the transparent render list so that we paint the items farther from the camera first. http://youtu.be/fEjZrwDKdi8
	MergeSortTransparentRenderList();

	// Now draw all transparent characters, sorted by distance from the camera.
	DrawCharacters(m_apRenderTransparentList, true);

	r.SetUniform("bDiffuse", false);

	// Render any bullet tracers that may have been created.
	float flBulletTracerTime = 0.1f;
	for (size_t i = 0; i < Game()->GetTracers().size(); i++)
	{
		if (Game()->GetTime() < Game()->GetTracers()[i].flTimeCreated + flBulletTracerTime)
		{
			Vector vecStart = Game()->GetTracers()[i].vecStart;
			Vector vecEnd = Game()->GetTracers()[i].vecEnd;

			r.SetUniform("vecColor", Vector4D(1, 0.9f, 0, 1));
			r.BeginRenderLines();
				r.Normal(Vector(0, 1, 0));
				r.Vertex(vecStart);
				r.Vertex(vecEnd);
			r.EndRender();
		}
	}

	// Render any puffs that may have been created.
	float flPuffTime = 0.3f;
	for (size_t i = 0; i < Game()->GetPuffs().size(); i++)
	{
		if (Game()->GetTime() < Game()->GetPuffs()[i].flTimeCreated + flPuffTime)
		{
			float flTimeCreated = Game()->GetPuffs()[i].flTimeCreated;
			float flTimeOver = Game()->GetPuffs()[i].flTimeCreated + flPuffTime;
			float flStartSize = 0.2f;
			float flEndSize = 2.0f;

			float flSize = Remap(Game()->GetTime(), flTimeCreated, flTimeOver, flStartSize, flEndSize);

			Vector vecOrigin = Game()->GetPuffs()[i].vecOrigin;

			int iOrange = (int)Remap(Game()->GetTime(), flTimeCreated, flTimeOver, 0, 255);
			r.SetUniform("vecColor", Color(255, iOrange, 0, 255));
			r.RenderBox(vecOrigin - Vector(1, 1, 1)*flSize, vecOrigin + Vector(1, 1, 1)*flSize);
		}
	}

	GraphDraw();

	RenderSeaweed();

	if (false)
	{
		CRenderingContext c(Game()->GetRenderer(), true);

		Vector camera = Game()->m_hPlayer->GetGlobalView();

		c.UseProgram("model");

		c.SetUniform("vecColor", Vector4D(0, 0, 0, 1));

		for (int k = 0; k < SPLINE_POINTS; k++)
		{
			float offset = (float)((2 * k) % 5) * (float)M_TAU / 5;

			Matrix4x4 m;
			m.SetTranslation(g_spline_points[k]);
			c.LoadTransform(m);
			c.RenderBox(Vector(-0.1f, -0.1f, -0.1f), Vector(0.1f, 0.1f, 0.1f));
		}

		c.ResetTransformations();

		c.BeginRenderLines();

		for (int k = 0; k < VArraySize(g_spline_segments)-1; k++)
		{
			Vector p0 = g_spline_segments[k];
			Vector p1 = g_spline_segments[k+1];

			c.Vertex(p0);
			c.Vertex(p1);
		}

		c.EndRender();

		float total_length = g_spline.GetTotalLength();

		Matrix4x4 m;
		m.SetTranslation(g_spline.ConstVelocitySplineAtTime(fmod(Application()->GetTime()*10, total_length / g_spline_speed), g_spline_speed));
		c.LoadTransform(m);
		c.RenderBox(Vector(-0.1f, -0.1f, -0.1f), Vector(0.1f, 0.1f, 0.1f));

		m.SetTranslation(g_spline.ConstVelocitySplineAtTime(fmod((Application()->GetTime()+0.1f)*10, total_length / g_spline_speed), g_spline_speed));
		c.LoadTransform(m);
		c.RenderBox(Vector(-0.1f, -0.1f, -0.1f), Vector(0.1f, 0.1f, 0.1f));

		m.SetTranslation(g_spline.ConstVelocitySplineAtTime(fmod((Application()->GetTime()+0.2f)*10, total_length / g_spline_speed), g_spline_speed));
		c.LoadTransform(m);
		c.RenderBox(Vector(-0.1f, -0.1f, -0.1f), Vector(0.1f, 0.1f, 0.1f));

		m.SetTranslation(g_spline.ConstVelocitySplineAtTime(fmod((Application()->GetTime()+0.3f)*10, total_length / g_spline_speed), g_spline_speed));
		c.LoadTransform(m);
		c.RenderBox(Vector(-0.1f, -0.1f, -0.1f), Vector(0.1f, 0.1f, 0.1f));
	}

	// Velocity field & Satellites
	{
		CRenderingContext c(Game()->GetRenderer(), true);

		Vector camera = Game()->m_hPlayer->GetGlobalView();

		c.UseProgram("model");

#if 0
		c.SetUniform("vecColor", Vector4D(0, 0, 0, 1));
		c.BeginRenderLines();

		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				Vector field_position = Vector(x, 2, y);
				Vector position = field_position * m_particle_scale;
				position.y = 2;
				c.Vertex(position);
				c.Vertex(position + VelocityField(field_position, Game()->GetTime()).Normalized());
			}
		}

		c.EndRender();

		for (int x = -10; x < 10; x++)
		{
			for (int y = -10; y < 10; y++)
			{
				Vector position = Vector(x*m_particle_scale, 2, y*m_particle_scale);
				Matrix4x4 m;
				m.SetTranslation(position);
				c.LoadTransform(m);
				c.RenderBox(Vector(-0.02f, -0.02f, -0.02f), Vector(0.02f, 0.02f, 0.02f));
			}
		}
#endif

		Matrix4x4 m;
		c.SetUniform("vecColor", Vector4D(1, 0.8f, 0, 1));

		for (int k = 0; k < VArraySize(m_stars); k++)
		{
			m.SetTranslation(m_stars[k]);
			c.LoadTransform(m);
			c.RenderBox(Vector(-0.2f, -0.2f, -0.2f), Vector(0.2f, 0.2f, 0.2f));
		}

		Vector4D path_colors[] = {
			Vector4D(1, 0, 0, 1),
			Vector4D(0, 1, 0, 1),
			Vector4D(0, 0, 1, 1),
			Vector4D(1, 1, 1, 1),
			Vector4D(0, 0, 0, 1),
			Vector4D(1, 1, 0, 1),
			Vector4D(0, 1, 1, 1),
			Vector4D(1, 0, 1, 1),
		};

		int path = 0;

		auto draw_particle = [&c, path_colors, this, &path](Vector position, Vector velocity, float radius)
		{
			c.ResetTransformations();

			c.SetUniform("vecColor", path_colors[path] * 0.5f);
			c.BeginRenderLines();
			for (int k = 0; k < m_particle_paths[path].size()-1; k++)
			{
				c.Vertex(m_particle_paths[path][k]);
				c.Vertex(m_particle_paths[path][k+1]);
			}
			c.EndRender();

			c.SetUniform("vecColor", path_colors[path]);

			c.BeginRenderLines();
				c.Vertex(position);
				c.Vertex(position + velocity);
			c.EndRender();

			Matrix4x4 m;
			m.SetTranslation(position);
			c.LoadTransform(m);

			c.RenderBox(Vector(-radius, -radius, -radius), Vector(radius, radius, radius));

			path++;
		};

#if 0
		for (int k = 0; k < VArraySize(m_particles); k++)
		{
			Vector v = m_particles[k];
			v = v * m_particle_scale;
			v.y = 2;
			draw_particle(v, Vector(), 0.1);
		}
#endif

		path = VArraySize(m_particles);

#if 1
		for (int k = 0; k < VArraySize(m_satellites); k++)
		{
			draw_particle(m_satellites[k].m_position, m_satellites[k].m_velocity, m_satellites[k].m_radius);
		}
#endif

		/*{
			c.ResetTransformations();
			c.SetUniform("vecColor", Vector4D(1.0, 0.8, 0.1, 1));

			Matrix4x4 m;
			float t = Game()->GetTime();
			m.SetTranslation(Vector(cos(t/1), 3, sin(t/1)));
			c.LoadTransform(m);
			c.RenderBox(Vector(-0.1, -0.1, -0.1), Vector(0.1, 0.1, 0.1));
		}*/

#if 0
		c.ResetTransformations();
		c.SetUniform("vecColor", Vector4D(1, 0.8, 0.2, 1));

		c.BeginRenderLines();
		auto f = [](float t) -> Vector
		{
			return Vector(-3 + 2/(1+exp(-2*t)), 0, t*t+(1.0f/3)*exp(-5*t));
		};
		for (int k = 0; k < 10000; k++)
		{
			float t = (float)k/100;

			c.Vertex(f(t)*m_particle_scale + Vector(0,2,0));
			c.Vertex(f(t+0.01f)*m_particle_scale + Vector(0,2,0));
		}
		c.EndRender();
#endif
	}

	pRenderer->FinishRendering(&r);

	// Call this last. Your rendered stuff won't appear on the screen until you call this.
	Application()->SwapBuffers();
}

void CGame::DrawCharacters(const std::vector<CCharacter*>& apRenderList, bool bTransparent)
{
	CRenderer* pRenderer = GetRenderer();

	// Loop through all characters, render them one at a time.
	// Start at the back of the list so that transparent entities use the painter's algorithm.
	for (size_t i = apRenderList.size()-1; i < apRenderList.size(); i--)
	{
		CCharacter* pCharacter = apRenderList[i];

		CRenderingContext c(pRenderer, true);

		c.SetBlend(BLEND_NONE);
		c.SetAlpha(1);

		// Set the color of the box to be rendered.
		c.SetUniform("vecColor", pCharacter->m_clrRender);

		if (pCharacter->m_iBillboardTexture)
		{
			c.SetBackCulling(false);
			c.SetUniform("bDiffuse", true);

			// Create a billboard by creating basis vectors. https://www.youtube.com/watch?v=puOTwCrEm7Q
			Vector vecForward, vecRight, vecUp;
			vecForward = pCharacter->GetGlobalOrigin() - pRenderer->GetCameraPosition();
			vecRight = -Vector(0, 1, 0).Cross(vecForward).Normalized();
			vecUp = vecForward.Cross(-vecRight).Normalized();

			if (pCharacter->m_bDrawTransparent)
			{
				c.SetAlpha(0.6f);
				c.SetBlend(BLEND_ALPHA);
			}

			Matrix4x4 transform = pCharacter->GetGlobalTransform();
			Matrix4x4 scale;
			scale.SetScale(pCharacter->m_vecScaling);
			transform *= scale;
			c.LoadTransform(transform);
			c.Translate(Vector(0, pCharacter->m_aabbSize.GetHeight()/2, 0)); // Move the character up so his feet don't stick in the ground.
			pCharacter->ShotEffect(&c);
			c.RenderBillboard(pCharacter->m_iBillboardTexture, pCharacter->m_aabbSize.vecMax.x, vecUp, vecRight);
		}
		else
		{
			c.SetUniform("bDiffuse", false);

			// The transform matrix holds all transformations for the player. Just pass it through to the renderer.
			// http://youtu.be/7pe1xYzFCvA
			c.Transform(pCharacter->GetGlobalTransform());

			if (pCharacter->m_bDrawTransparent)
			{
				c.SetAlpha(0.6f);
				c.SetBlend(BLEND_ALPHA);
			}

			if (pCharacter->m_iTexture)
			{
				c.SetUniform("bDiffuse", true);
				c.BindTexture(pCharacter->m_iTexture);
			}

			// Render the player-box
			c.RenderBox(pCharacter->m_aabbSize.vecMin, pCharacter->m_aabbSize.vecMax);
		}
	}
}

// Sort our render list using the divide and conquer technique knows as Merge Sort.
// http://youtu.be/fEjZrwDKdi8
void MergeSortRenderSubList(std::vector<CCharacter*>& apRenderList, size_t iStart, size_t iEnd)
{
	// iStart is the index of the first index that we are to sort. iEnd is the index+1 of the last index we are to sort.
	size_t iLength = iEnd - iStart;

	if (iLength <= 1)
	{
		// We are in a base case of one item. We're sorted! Return.
		return;
	}
	else if (iLength == 2)
	{
		// We are in a base case of two items. If the first one is bigger than the second, swap them.
		float flLeftDistanceSqr = (apRenderList[iStart]->GetGlobalOrigin() - Game()->GetRenderer()->GetCameraPosition()).LengthSqr();
		float flRightDistanceSqr = (apRenderList[iStart+1]->GetGlobalOrigin() - Game()->GetRenderer()->GetCameraPosition()).LengthSqr();

		// We can compare square distances just like regular distances, and they're faster to calculate. http://www.youtube.com/watch?v=DxmGxkhhluU
		if (flLeftDistanceSqr > flRightDistanceSqr)
			std::swap(apRenderList[iStart], apRenderList[iStart+1]);

		// Now we're sorted!
		return;
	}

	// We aren't in a base case yet. Split the list in two.
	size_t iMiddle = (iStart + iEnd)/2;

	// Sort the two sub-lists by calling this function recursively.
	MergeSortRenderSubList(apRenderList, iStart, iMiddle);
	MergeSortRenderSubList(apRenderList, iMiddle, iEnd);

	// Merge the two sub-lists together by plucking off the lowest element.
	// First make a copy of the list.
	std::vector<CCharacter*> apRenderListCopy = apRenderList;

	size_t iLeft = iStart;
	size_t iRight = iMiddle;
	size_t iOutput = iStart;
	while (true)
	{
		float flLeftDistanceSqr = (apRenderListCopy[iLeft]->GetGlobalOrigin() - Game()->GetRenderer()->GetCameraPosition()).LengthSqr();
		float flRightDistanceSqr = 0;
		if (iRight != iEnd)
			flRightDistanceSqr = (apRenderListCopy[iRight]->GetGlobalOrigin() - Game()->GetRenderer()->GetCameraPosition()).LengthSqr();

		// We can compare square distances just like regular distances, and they're faster to calculate. http://www.youtube.com/watch?v=DxmGxkhhluU
		bool bUseLeft = flLeftDistanceSqr < flRightDistanceSqr;
		if (iRight == iEnd)
			bUseLeft = true;
		else if (iLeft == iMiddle)
			bUseLeft = false;

		if (bUseLeft)
			apRenderList[iOutput++] = apRenderListCopy[iLeft++];
		else
			apRenderList[iOutput++] = apRenderListCopy[iRight++];

		// If we're reached the end of both sub-lists, break from the loop.
		if (iLeft == iMiddle && iRight == iEnd)
			break;
	}

	// Our sub-list is sorted! Return.
}

void CGame::MergeSortTransparentRenderList()
{
	MergeSortRenderSubList(m_apRenderTransparentList, 0, m_apRenderTransparentList.size());
}

// The Game Loop http://www.youtube.com/watch?v=c4b9lCfSDQM
void CGame::GameLoop()
{
	m_hPlayer = CreateCharacter();

	// Initialize the box's position etc
	m_hPlayer->SetGlobalOrigin(Point(0, 0, 0));
	m_hPlayer->m_vecMovement = Vector(0, 0, 0);
	m_hPlayer->m_vecMovementGoal = Vector(0, 0, 0);
	m_hPlayer->m_vecVelocity = Vector(0, 0, 0);
	m_hPlayer->m_vecGravity = Vector(0, -10, 0);
	m_hPlayer->m_flSpeed = 15;
	m_hPlayer->m_clrRender = Color(0.8f, 0.4f, 0.2f, 1.0f);
	m_hPlayer->m_bHitByTraces = false;
	m_hPlayer->m_aabbSize = AABB(-Vector(0.5f, 0, 0.5f), Vector(0.5f, 2, 0.5f));
	m_hPlayer->m_bTakesDamage = true;

	Vector vecMonsterMin = Vector(-1, 0, -1);
	Vector vecMonsterMax = Vector(1, 2, 1);

	/*
	CCharacter* pTarget1 = CreateCharacter();
	pTarget1->SetTransform(Vector(2, 2, 2), 0, Vector(0, 1, 0), Vector(6, 0, 6));
	pTarget1->m_aabbSize.vecMin = vecMonsterMin;
	pTarget1->m_aabbSize.vecMax = vecMonsterMax;
	pTarget1->m_iBillboardTexture = m_iMonsterTexture;
	pTarget1->m_bEnemyAI = true;
	pTarget1->m_bTakesDamage = true;

	CCharacter* pTarget2 = CreateCharacter();
	pTarget2->SetTransform(Vector(2, 2, 2), 0, Vector(0, 1, 0), Vector(6, 0, -6));
	pTarget2->m_aabbSize.vecMin = vecMonsterMin;
	pTarget2->m_aabbSize.vecMax = vecMonsterMax;
	pTarget2->m_iBillboardTexture = m_iMonsterTexture;
	pTarget2->m_bEnemyAI = true;
	pTarget2->m_bTakesDamage = true;

	CCharacter* pTarget3 = CreateCharacter();
	pTarget3->SetTransform(Vector(2, 2, 2), 0, Vector(0, 1, 0), Vector(-6, 0, 8));
	pTarget3->m_aabbSize.vecMin = vecMonsterMin;
	pTarget3->m_aabbSize.vecMax = vecMonsterMax;
	pTarget3->m_iBillboardTexture = m_iMonsterTexture;
	pTarget3->m_bEnemyAI = true;
	pTarget3->m_bTakesDamage = true;
	*/

	Vector vecPropMin = Vector(-.1f, 0, -.1f);
	Vector vecPropMax = Vector(.1f, .2f, .1f);

	mtsrand(0);

	/*for (int i = 0; i < 800; i++)
	{
		float rand1 = (float)(mtrand()%1000)/1000; // [0, 1]
		float rand2 = (float)(mtrand()%1000)/1000; // [0, 1]

		float theta = rand1 * 2.0f * (float)M_PI;
		float radius = sqrt(rand2);

		Vector position = Vector(radius * cos(theta), 0, radius * sin(theta));
		position = position * 50;

		CCharacter* pProp = CreateCharacter();
		pProp->SetTransform(Vector(1, 1, 1), 0, Vector(0, 1, 0), position);
		pProp->m_aabbSize.vecMin = vecPropMin;
		pProp->m_aabbSize.vecMax = vecPropMax;
		pProp->m_clrRender = Color(0.4f, 0.8f, 0.2f, 1.0f);
		pProp->m_iTexture = m_iCrateTexture;
	}*/

	CRenderingContext c(GetRenderer());
	c.RenderBox(Vector(-1, 0, -1), Vector(1, 2, 1));
	c.CreateVBO(m_iMeshVB, m_iMeshSize);

	float flPreviousTime = 0;
	float flCurrentTime = Application()->GetTime();

	while (true)
	{
		// flCurrentTime will be lying around from last frame. It's now the previous time.
		flPreviousTime = flCurrentTime;
		flCurrentTime = Application()->GetTime();

		float dt = flCurrentTime - flPreviousTime;

		// Keep dt from growing too large.
		if (dt > 0.15f)
			dt = 0.15f;

		Update(dt);

		Draw();
	}
}

// Create a character and add him into our entity list.
// Entity list explained here: http://youtu.be/V6vq0PRFKgk
CCharacter* CGame::CreateCharacter()
{
	size_t iSpot = ~0;

	// Find a spot in my entity list that's empty.
	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		if (!m_apEntityList[i])
		{
			iSpot = i;
			break;
		}
	}

	if (iSpot == ~0)
		// Couldn't find a spot for the new guy! Return null instead.
		return nullptr;

	m_apEntityList[iSpot] = new CCharacter();

	static int iParity = 0;
	m_apEntityList[iSpot]->m_iParity = iParity++;
	m_apEntityList[iSpot]->m_iIndex = iSpot;

	return m_apEntityList[iSpot];
}

// Remove a character from the entity list.
void CGame::RemoveCharacter(CCharacter* pCharacter)
{
	size_t iSpot = ~0;

	// Find a spot in my entity list that's empty.
	for (size_t i = 0; i < MAX_CHARACTERS; i++)
	{
		if (m_apEntityList[i] == pCharacter)
		{
			iSpot = i;
			break;
		}
	}

	if (iSpot == ~0)
		// Couldn't find this guy in our entity list! Do nothing.
		return;

	delete m_apEntityList[iSpot];
	m_apEntityList[iSpot] = nullptr;
}

CCharacter* CGame::GetCharacterIndex(size_t i)
{
	return m_apEntityList[i];
}

void CGame::GraphReset()
{
	m_eGraphStep = GRAPHSTEP_BEGIN;
	m_aiUnvisitedNodes.clear();
	m_aiPathStack.clear();
	m_iCurrentNode = -1;

	m_Graph = CGraph();
}

bool smaller_weight(const node_t& l, const node_t& r)
{
	auto& graph = Game()->m_Graph;
	return graph.GetNode(l)->path_weight > graph.GetNode(r)->path_weight;
}

void CGame::GraphStep()
{
	if (m_eGraphStep == GRAPHSTEP_BEGIN)
	{
		m_iCurrentNode = 0;
		m_Graph.GetNode(m_iCurrentNode)->seen = true;
		m_Graph.GetNode(m_iCurrentNode)->path_weight = 0;

		for (int i = 1; i < m_Graph.GetNumNodes(); i++)
			m_aiUnvisitedNodes.push_back(i);

		m_eGraphStep = GRAPHSTEP_CALCULATENEIGHBORS;
	}
	else if (m_eGraphStep == GRAPHSTEP_CALCULATENEIGHBORS)
	{
		CGraph::CNode* current_node = m_Graph.GetNode(m_iCurrentNode);

		size_t i;
		for (i = 0; i < current_node->edges.size(); i++)
		{
			edge_t edge = current_node->edges[i];

			node_t test_node = m_Graph.FollowEdge(m_iCurrentNode, edge);

			if (m_Graph.GetNode(test_node)->seen)
				continue;

			float g_weight = m_Graph.GetEdge(edge)->weight + current_node->path_weight;
			float h_weight = (m_Graph.GetNode(test_node)->debug_position - m_pTargetNode->debug_position).Length();

			float f_weight = g_weight + h_weight;

			if (f_weight < m_Graph.GetNode(test_node)->path_weight)
			{
				m_Graph.GetNode(test_node)->path_weight = f_weight;
				m_Graph.GetNode(test_node)->path_from = m_iCurrentNode;
			}
		}

		// We made changes to our node weights. Make sure it's still a heap by remaking the heap.
		std::make_heap(m_aiUnvisitedNodes.begin(), m_aiUnvisitedNodes.end(), smaller_weight);

		m_eGraphStep = GRAPHSTEP_FINDLOWEST;
	}
	else if (m_eGraphStep == GRAPHSTEP_FINDLOWEST)
	{
		if (!m_aiUnvisitedNodes.size())
			return;

		std::pop_heap(m_aiUnvisitedNodes.begin(), m_aiUnvisitedNodes.end(), smaller_weight);

		node_t lowest_path_node = m_aiUnvisitedNodes.back();
		float lowest_path_weight = m_Graph.GetNode(lowest_path_node)->path_weight;

		m_aiUnvisitedNodes.pop_back();

		if (lowest_path_node < 0)
			return;

		m_iCurrentNode = lowest_path_node;
		m_Graph.GetNode(m_iCurrentNode)->seen = true;

		if (m_Graph.GetNode(lowest_path_node) == m_pTargetNode)
		{
			m_eGraphStep = GRAPHSTEP_RECONSTRUCT;
			return;
		}

		m_eGraphStep = GRAPHSTEP_CALCULATENEIGHBORS;
	}
	else if (m_eGraphStep == GRAPHSTEP_RECONSTRUCT)
	{
		node_t current_node = m_iCurrentNode;

		m_aiPathStack.push_back(current_node);

		while (m_Graph.GetNode(current_node)->path_from != ~0)
		{
			current_node = m_Graph.GetNode(current_node)->path_from;
			m_aiPathStack.push_back(current_node);
		}
	}
}

void CGame::GraphComplete()
{
	GraphReset();

	node_t current_node_index = 0;
	m_Graph.GetNode(current_node_index)->seen = true;
	m_Graph.GetNode(current_node_index)->path_weight = 0;

	for (int i = 1; i < m_Graph.GetNumNodes(); i++)
		m_aiUnvisitedNodes.push_back(i);

	while (m_aiUnvisitedNodes.size())
	{
		CGraph::CNode* current_node = m_Graph.GetNode(current_node_index);

		size_t i;
		for (i = 0; i < current_node->edges.size(); i++)
		{
			edge_t edge = current_node->edges[i];

			node_t test_node = m_Graph.FollowEdge(current_node_index, edge);

			if (m_Graph.GetNode(test_node)->seen)
				continue;

			float g_weight = m_Graph.GetEdge(edge)->weight + current_node->path_weight;
			float h_weight = (m_Graph.GetNode(test_node)->debug_position - m_pTargetNode->debug_position).Length();

			float f_weight = g_weight + h_weight;

			if (f_weight < m_Graph.GetNode(test_node)->path_weight)
			{
				m_Graph.GetNode(test_node)->path_weight = f_weight;
				m_Graph.GetNode(test_node)->path_from = current_node_index;
			}
		}

		// We made changes to our node weights. Make sure it's still a heap by remaking the heap.
		std::make_heap(m_aiUnvisitedNodes.begin(), m_aiUnvisitedNodes.end(), smaller_weight);

		// Pop the smallest item off the heap.
		std::pop_heap(m_aiUnvisitedNodes.begin(), m_aiUnvisitedNodes.end(), smaller_weight);

		node_t lowest_path_node = m_aiUnvisitedNodes.back();
		float lowest_path_weight = m_Graph.GetNode(lowest_path_node)->path_weight;

		m_aiUnvisitedNodes.pop_back();

		if (lowest_path_node < 0)
			return;

		current_node_index = lowest_path_node;
		m_Graph.GetNode(current_node_index)->seen = true;

		if (m_Graph.GetNode(lowest_path_node) == m_pTargetNode)
		{
			m_aiPathStack.push_back(current_node_index);

			while (m_Graph.GetNode(current_node_index)->path_from != ~0)
			{
				current_node_index = m_Graph.GetNode(current_node_index)->path_from;
				m_aiPathStack.push_back(current_node_index);
			}

			return;
		}
	}
}

void CGame::GraphDraw()
{
	CRenderingContext c(GetRenderer(), true);

	for (int i = 0; i < m_Graph.GetNumNodes(); i++)
	{
		CGraph::CNode* node = m_Graph.GetNode(i);

		{
			if (m_iCurrentNode == i)
				c.SetUniform("vecColor", Color(0, 255, 0, 255));
			else if (node == m_pTargetNode)
				c.SetUniform("vecColor", Color(255, 120, 0, 255));
			else if (node->path_weight < 99999999999)
			{
				int r = (int)Remap(node->path_weight, 0, 10, 50, 255);
				c.SetUniform("vecColor", Color(r, 50, 50, 255));
			}
			else
			{
				c.SetUniform("vecColor", Color(255, 255, 255, 255));

				for (size_t j = 0; j < m_aiUnvisitedNodes.size(); j++)
				{
					if (m_aiUnvisitedNodes[j] == i)
					{
						c.SetUniform("vecColor", Color(0, 0, 255, 255));
						break;
					}
				}
			}
		}

		c.RenderBox(node->debug_position - Vector(1, 1, 1), node->debug_position + Vector(1, 1, 1));

		if (node->seen)
		{
			c.SetUniform("vecColor", Color(0, 0, 0, 255));
			c.RenderBox(node->debug_position + Vector(1, 1, -1) - Vector(0.2f, 0.2f, 0.2f), node->debug_position + Vector(1, 1, -1) + Vector(0.2f, 0.2f, 0.2f));
		}
	}

	c.SetUniform("bLighted", false);

	for (int i = 0; i < m_Graph.GetNumEdges(); i++)
	{
		CGraph::CEdge* edge = m_Graph.GetEdge(i);
		CGraph::CNode* node1 = m_Graph.GetNode(edge->first);
		CGraph::CNode* node2 = m_Graph.GetNode(edge->second);

		bool in_path = false;
		for (int j = 0; j < ((int)m_aiPathStack.size())-1; j++)
		{
			if (m_aiPathStack[j] == edge->first && m_aiPathStack[j+1] == edge->second)
			{
				in_path = true;
				break;
			}

			if (m_aiPathStack[j] == edge->second && m_aiPathStack[j+1] == edge->first)
			{
				in_path = true;
				break;
			}
		}

		if (in_path)
			c.SetUniform("vecColor", Color(0, 0, 255, 255));
		else
			c.SetUniform("vecColor", Color((int)Remap(edge->weight, 1, 8, 0, 255), 0, 0, 255));

		c.BeginRenderLines();
			c.Vertex(node1->debug_position + Vector(0, 0.1f, 0));
			c.Vertex(node2->debug_position + Vector(0, 0.1f, 0));
		c.EndRender();

		Vector path_start, path_end;
		bool show_path = false;

		if (node1->path_from == edge->second)
		{
			path_start = node1->debug_position;
			path_end = node2->debug_position;
			show_path = true;
		}
		else if (node2->path_from == edge->first)
		{
			path_start = node2->debug_position;
			path_end = node1->debug_position;
			show_path = true;
		}

		if (show_path)
		{
			float lerp = fmod(GetTime(), 1.0f);
			Vector position = path_start * (1-lerp) + path_end * lerp;
			c.RenderBox(position - Vector(0.2f, 0.2f, 0.2f), position + Vector(0.2f, 0.2f, 0.2f));
		}
	}
}
