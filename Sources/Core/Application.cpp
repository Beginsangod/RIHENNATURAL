#include "Application.h"
#include <iostream>
#include <chrono>

#include "Core/Math/MathConstants.h"

namespace nkentseu {

    Application::Application(const ApplicationProperties& props) 
    : mProperties(props), mTransformMatrix(math::Matrix3x3::Identity()) {
        Log.Info("Application created with properties:");
        Log.Info("  Title: {0}", mProperties.windowTitle);
        Log.Info("  Size: {0}x{1}", mProperties.windowWidth, mProperties.windowHeight);
        Log.Info("  Name: {0}", mProperties.applicationName);
    }

    Application::~Application() {
        Log.Info("Application destroyed");
        Close();
    }

    bool Application::Initialize() {
        if (mIsInitialized) return true;

        Log.Info("Initializing application...");

        // Initialiser SDL
        int sdlResult = SDL_Init(SDL_INIT_VIDEO);
        if (sdlResult < 0) {
            Log.Error("SDL_Init failed: {0}", SDL_GetError());
            return false;
        }

        // Configurer les flags de la fenêtre
        Uint32 windowFlags = 0;
        if (mProperties.resizable) windowFlags |= SDL_WINDOW_RESIZABLE;
        if (mProperties.borderless) windowFlags |= SDL_WINDOW_BORDERLESS;
        if (mProperties.highDPI) windowFlags |= SDL_WINDOW_HIGH_PIXEL_DENSITY;
        if (mProperties.fullscreen) windowFlags |= SDL_WINDOW_FULLSCREEN;

        // Créer la fenêtre
        mWindow = SDL_CreateWindow(mProperties.windowTitle.c_str(), 
                                mProperties.windowWidth, 
                                mProperties.windowHeight, 
                                windowFlags);
        if (!mWindow) {
            Log.Error("SDL_CreateWindow failed: {0}", SDL_GetError());
            SDL_Quit();
            return false;
        }

        // Positionner la fenêtre si nécessaire
        if (mProperties.windowPosition.x != SDL_WINDOWPOS_CENTERED || 
            mProperties.windowPosition.y != SDL_WINDOWPOS_CENTERED) {
            SDL_SetWindowPosition(mWindow, 
                                static_cast<int>(mProperties.windowPosition.x), 
                                static_cast<int>(mProperties.windowPosition.y));
        }

        // Configurer les flags du renderer
        // Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE;
        // (void)rendererFlags; // Pour éviter le warning
        // SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC | SDL_RENDERER_TARGETTEXTURE
        // if (mProperties.vsync) rendererFlags |= 0x00000004; // SDL_RENDERER_PRESENTVSYNC

        // Créer le renderer
        mRenderer = SDL_CreateRenderer(mWindow, nullptr);
        if (!mRenderer) {
            Log.Error("SDL_CreateRenderer failed: {0}", SDL_GetError());
            SDL_DestroyWindow(mWindow);
            SDL_Quit();
            return false;
        }

        mIsInitialized = true;
        Log.Info("Application initialized successfully");
        return true;
    }

    void Application::Close() {
        Log.Info("Closing application...");
        
        if (mRenderer) {
            SDL_DestroyRenderer(mRenderer);
            mRenderer = nullptr;
            Log.Debug("Renderer destroyed");
        }
        if (mWindow) {
            SDL_DestroyWindow(mWindow);
            mWindow = nullptr;
            Log.Debug("Window destroyed");
        }
        SDL_Quit();
        mIsInitialized = false;
        
        Log.Info("Application closed");
    }

    void Application::Run() {
        if (!mIsInitialized) {
            Log.Error("Cannot run application: not initialized");
            return;
        }

        mIsRunning = true;
        Log.Info("Starting main loop");
        
        // Appeler le setup utilisateur
        Log.Debug("Calling user Setup()");
        Setup();

        auto lastTime = std::chrono::high_resolution_clock::now();
        uint64_t frameCount = 0;
        
        // Boucle principale
        while (mIsRunning) {
            frameCount++;
            auto currentTime = std::chrono::high_resolution_clock::now();
            float deltaTime = std::chrono::duration<float>(currentTime - lastTime).count();
            lastTime = currentTime;

            ProcessEvents();
            UpdateInput();
            
            // Mise à jour utilisateur
            Update(deltaTime);

            // Rendu
            
            // Dessin utilisateur
            Draw();
            
            SDL_RenderPresent(mRenderer);

            // Limiter à ~60 FPS
            SDL_Delay(16);
        }

        Log.Info("Main loop ended after {0} frames", frameCount);
    }

    void Application::ProcessEvents() {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_EVENT_QUIT:
                    mIsRunning = false;
                    break;
                default:
                    break;
            }
        }
    }

    void Application::UpdateInput() {
        events::Mouse::Update();
        events::Keyboard::Update();
    }

    void Application::Exit() {
        mIsRunning = false;
    }

    void Application::SetWindowSize(int width, int height) {
        mProperties.windowWidth = width;
        mProperties.windowHeight = height;
        if (mWindow) {
            SDL_SetWindowSize(mWindow, width, height);
        }
    }

    void Application::SetWindowTitle(const char* title) {
        mProperties.windowTitle = title;
        if (mWindow) {
            SDL_SetWindowTitle(mWindow, title);
        }
    }

    // Méthodes graphiques
    void Application::Background(const graphics::Color& color, bool useInternalClear) {
        SDL_SetRenderDrawColor(mRenderer, color.r, color.g, color.b, color.a);

        if (useInternalClear) {
            SDL_RenderClear(mRenderer);
        }

        SDL_FRect rect = {0, 0, (float)mProperties.windowWidth, (float)mProperties.windowHeight};
        SDL_RenderFillRect(mRenderer, &rect);
    }

    void Application::Fill(const graphics::Color& color) {
        mFillColor = color;
        mUseFill = true;
    }

    void Application::Stroke(const graphics::Color& color) {
        mStrokeColor = color;
        mUseStroke = true;
    }

    void Application::NoFill() {
        mUseFill = false;
    }

    void Application::NoStroke() {
        mUseStroke = false;
    }

    void Application::ThickLine(const math::Vector2& start, const math::Vector2& end, float thickness) {
        if (thickness <= 1.0f || !mUseStroke) {
            math::Vector2 transformedStart = TransformPoint(start);
            math::Vector2 transformedEnd = TransformPoint(end);
            SDL_RenderLine(mRenderer, transformedStart.x, transformedStart.y, transformedEnd.x, transformedEnd.y);
            return;
        }
        
        SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
        
        // Transformer les points
        math::Vector2 transformedStart = TransformPoint(start);
        math::Vector2 transformedEnd = TransformPoint(end);
        
        // Calculer la direction et la perpendicular
        math::Vector2 direction = (transformedEnd - transformedStart);
        float length = direction.Magnitude();
        if (length == 0.0f) return;
        
        direction = direction / length;
        math::Vector2 perpendicular(-direction.y, direction.x);
        math::Vector2 offset = perpendicular * (thickness / 2.0f);
        
        // Points du quadrilatère
        math::Vector2 points[4] = {
            transformedStart + offset,
            transformedStart - offset,
            transformedEnd - offset,
            transformedEnd + offset
        };
        
        // Dessiner le quadrilatère rempli
        for (int y = 0; y < thickness; y++) {
            for (int x = 0; x < length; x++) {
                math::Vector2 point = transformedStart + direction * static_cast<float>(x) + perpendicular * (static_cast<float>(y) - thickness/2.0f);
                
                // Vérifier si le point est dans le quadrilatère
                if (IsPointInQuadrilateral(point, points)) {
                    SDL_RenderPoint(mRenderer, point.x, point.y);
                }
            }
        }
        
        // Dessiner les extrémités arrondies
        DrawCircleInternal(start, thickness / 2.0f);
        DrawCircleInternal(end, thickness / 2.0f);
    }

    // Méthode interne pour dessiner un cercle sans circularité
    void Application::DrawCircleInternal(const math::Vector2& center, float radius) {
        math::Vector2 transformedCenter = TransformPoint(center);
        math::Vector2 scale = math::Vector2(mTransformMatrix.m[0][0], mTransformMatrix.m[1][1]);
        float scaledRadiusX = radius * scale.x;
        float scaledRadiusY = radius * scale.y;
        
        if (mUseFill) {
            SDL_SetRenderDrawColor(mRenderer, mFillColor.r, mFillColor.g, mFillColor.b, mFillColor.a);
            
            // Implémentation directe sans appeler Ellipse
            const int segments = 32;
            std::vector<math::Vector2> points;
            
            for (int i = 0; i <= segments; i++) {
                float angle = math::TWO_PI * i / segments;
                math::Vector2 point = {
                    transformedCenter.x + scaledRadiusX * std::cos(angle),
                    transformedCenter.y + scaledRadiusY * std::sin(angle)
                };
                points.push_back(point);
            }
            
            Polygon(points, true);
        }
        
        if (mUseStroke && mStrokeWeight > 0) {
            const int segments = 32;
            math::Vector2 previousPoint;
            
            for (int i = 0; i <= segments; i++) {
                float angle = math::TWO_PI * i / segments;
                math::Vector2 point = {
                    transformedCenter.x + scaledRadiusX * std::cos(angle),
                    transformedCenter.y + scaledRadiusY * std::sin(angle)
                };
                
                if (i > 0) {
                    if (mStrokeWeight > 1.0f) {
                        // Utiliser une ligne épaisse directe
                        math::Vector2 dir = (point - previousPoint);
                        float length = dir.Magnitude();
                        if (length > 0) {
                            dir = dir / length;
                            math::Vector2 perpendicular(-dir.y, dir.x);
                            math::Vector2 offset = perpendicular * (mStrokeWeight / 2.0f);
                            
                            math::Vector2 p1 = previousPoint + offset;
                            math::Vector2 p2 = previousPoint - offset;
                            math::Vector2 p3 = point - offset;
                            math::Vector2 p4 = point + offset;
                            
                            std::vector<math::Vector2> quad = {p1, p2, p3, p4};
                            Polygon(quad, true);
                        }
                    } else {
                        SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
                        SDL_RenderLine(mRenderer, previousPoint.x, previousPoint.y, point.x, point.y);
                    }
                }
                
                previousPoint = point;
            }
        }
    }

    bool Application::IsPointInQuadrilateral(const math::Vector2& point, const math::Vector2 quad[4]) {
        // Implémentation simple de test de point dans quadrilatère
        float totalArea = 0.0f;
        float area1 = 0.0f, area2 = 0.0f, area3 = 0.0f, area4 = 0.0f;
        
        // Calculer les aires des 4 triangles
        area1 = std::abs((quad[0].x*(quad[1].y-point.y) + quad[1].x*(point.y-quad[0].y) + point.x*(quad[0].y-quad[1].y)) / 2.0f);
        area2 = std::abs((quad[1].x*(quad[2].y-point.y) + quad[2].x*(point.y-quad[1].y) + point.x*(quad[1].y-quad[2].y)) / 2.0f);
        area3 = std::abs((quad[2].x*(quad[3].y-point.y) + quad[3].x*(point.y-quad[2].y) + point.x*(quad[2].y-quad[3].y)) / 2.0f);
        area4 = std::abs((quad[3].x*(quad[0].y-point.y) + quad[0].x*(point.y-quad[3].y) + point.x*(quad[3].y-quad[0].y)) / 2.0f);
        
        totalArea = area1 + area2 + area3 + area4;
        
        // Aire du quadrilatère
        float quadArea = std::abs(
            (quad[0].x*(quad[1].y-quad[2].y) + quad[1].x*(quad[2].y-quad[0].y) + quad[2].x*(quad[0].y-quad[1].y)) / 2.0f +
            (quad[0].x*(quad[2].y-quad[3].y) + quad[2].x*(quad[3].y-quad[0].y) + quad[3].x*(quad[0].y-quad[2].y)) / 2.0f
        );
        
        return std::abs(totalArea - quadArea) < 0.1f;
    }

    void Application::Rect(const math::Vector2& position, const math::Vector2& size) {
        math::Vector2 transformedPos = TransformPoint(position);
        math::Vector2 transformedSize = mTransformMatrix.TransformVector(size);
        
        if (mUseFill) {
            SDL_SetRenderDrawColor(mRenderer, mFillColor.r, mFillColor.g, mFillColor.b, mFillColor.a);
            SDL_FRect rect = {transformedPos.x, transformedPos.y, transformedSize.x, transformedSize.y};
            SDL_RenderFillRect(mRenderer, &rect);
        }
        
        if (mUseStroke && mStrokeWeight > 0) {
            // Créer les 4 points du rectangle transformés
            math::Vector2 p1 = transformedPos;
            math::Vector2 p2 = {transformedPos.x + transformedSize.x, transformedPos.y};
            math::Vector2 p3 = {transformedPos.x + transformedSize.x, transformedPos.y + transformedSize.y};
            math::Vector2 p4 = {transformedPos.x, transformedPos.y + transformedSize.y};
            
            // Dessiner les 4 côtés avec épaisseur
            if (mStrokeWeight > 1.0f) {
                ThickLine(p1, p2, mStrokeWeight);
                ThickLine(p2, p3, mStrokeWeight);
                ThickLine(p3, p4, mStrokeWeight);
                ThickLine(p4, p1, mStrokeWeight);
            } else {
                SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
                SDL_FRect rect = {transformedPos.x, transformedPos.y, transformedSize.x, transformedSize.y};
                SDL_RenderRect(mRenderer, &rect);
            }
        }
    }

    void Application::Line(const math::Vector2& start, const math::Vector2& end) {
        math::Vector2 transformedStart = TransformPoint(start);
        math::Vector2 transformedEnd = TransformPoint(end);
        
        if (mUseStroke) {
            if (mStrokeWeight > 1.0f) {
                ThickLine(transformedStart, transformedEnd, mStrokeWeight);
            } else {
                SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
                SDL_RenderLine(mRenderer, transformedStart.x, transformedStart.y, transformedEnd.x, transformedEnd.y);
            }
        }
    }

    // Implémentation des nouvelles méthodes de dessin
    void Application::StrokeWeight(float weight) {
        mStrokeWeight = weight;
        // SDL3 n'a pas cette fonction, on la gère nous-mêmes
        // SDL_SetRenderDrawStrokeWidth(mRenderer, weight); // ← SUPPRIMEZ cette ligne
    }

    void Application::Ellipse(const math::Vector2& center, float radiusX, float radiusY) {
        math::Vector2 transformedCenter = TransformPoint(center);
        math::Vector2 scale = math::Vector2(mTransformMatrix.m[0][0], mTransformMatrix.m[1][1]);
        float scaledRadiusX = radiusX * scale.x;
        float scaledRadiusY = radiusY * scale.y;
        
        if (mUseFill) {
            SDL_SetRenderDrawColor(mRenderer, mFillColor.r, mFillColor.g, mFillColor.b, mFillColor.a);
            
            // Calculer les bounds transformés
            math::Vector2 minBound = transformedCenter - math::Vector2(scaledRadiusX, scaledRadiusY);
            math::Vector2 maxBound = transformedCenter + math::Vector2(scaledRadiusX, scaledRadiusY);
            
            for (int y = minBound.y; y <= maxBound.y; y++) {
                for (int x = minBound.x; x <= maxBound.x; x++) {
                    float dx = (x - transformedCenter.x) / scaledRadiusX;
                    float dy = (y - transformedCenter.y) / scaledRadiusY;
                    if (dx*dx + dy*dy <= 1.0f) {
                        SDL_RenderPoint(mRenderer, x, y);
                    }
                }
            }
        }
        
        if (mUseStroke && mStrokeWeight > 0) {
            const int segments = 64;
            math::Vector2 previousPoint;
            
            for (int i = 0; i <= segments; i++) {
                float angle = math::TWO_PI * i / segments;
                math::Vector2 point = {
                    transformedCenter.x + scaledRadiusX * std::cos(angle),
                    transformedCenter.y + scaledRadiusY * std::sin(angle)
                };
                
                if (i > 0) {
                    if (mStrokeWeight > 1.0f) {
                        ThickLine(previousPoint, point, mStrokeWeight);
                    } else {
                        SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
                        SDL_RenderLine(mRenderer, previousPoint.x, previousPoint.y, point.x, point.y);
                    }
                }
                
                previousPoint = point;
            }
        }
    }

    void Application::Polygon(const std::vector<math::Vector2>& vertices, bool filled) {
        if (vertices.size() < 3) return;
        
        // Transformer tous les points
        std::vector<math::Vector2> transformedVertices;
        for (const auto& vertex : vertices) {
            transformedVertices.push_back(TransformPoint(vertex));
        }
        
        if (filled && mUseFill) {
            SDL_SetRenderDrawColor(mRenderer, mFillColor.r, mFillColor.g, mFillColor.b, mFillColor.a);
            
            // Remplissage de polygone
            float minX = transformedVertices[0].x, maxX = transformedVertices[0].x;
            float minY = transformedVertices[0].y, maxY = transformedVertices[0].y;
            
            for (const auto& vertex : transformedVertices) {
                minX = std::min(minX, vertex.x);
                maxX = std::max(maxX, vertex.x);
                minY = std::min(minY, vertex.y);
                maxY = std::max(maxY, vertex.y);
            }
            
            for (int y = minY; y <= maxY; y++) {
                for (int x = minX; x <= maxX; x++) {
                    if (IsPointInPolygon(math::Vector2(x, y), transformedVertices)) {
                        SDL_RenderPoint(mRenderer, x, y);
                    }
                }
            }
        }
        
        if (mUseStroke && mStrokeWeight > 0) {
            // Dessiner le contour du polygone avec épaisseur
            for (size_t i = 0; i < transformedVertices.size(); i++) {
                size_t j = (i + 1) % transformedVertices.size();
                if (mStrokeWeight > 1.0f) {
                    ThickLine(transformedVertices[i], transformedVertices[j], mStrokeWeight);
                } else {
                    SDL_SetRenderDrawColor(mRenderer, mStrokeColor.r, mStrokeColor.g, mStrokeColor.b, mStrokeColor.a);
                    SDL_RenderLine(mRenderer, transformedVertices[i].x, transformedVertices[i].y, transformedVertices[j].x, transformedVertices[j].y);
                }
            }
        }
    }

    void Application::Triangle(const math::Vector2& p1, const math::Vector2& p2, const math::Vector2& p3) {
        std::vector<math::Vector2> vertices = {p1, p2, p3};
        Polygon(vertices, mUseFill);
    }

    void Application::Quad(const math::Vector2& p1, const math::Vector2& p2, const math::Vector2& p3, const math::Vector2& p4) {
        std::vector<math::Vector2> vertices = {p1, p2, p3, p4};
        Polygon(vertices, mUseFill);
    }

    void Application::Arc(const math::Vector2& center, float radius, 
                         float startAngle, float endAngle, int mode) {
        if (radius <= 0) return;
        
        math::Vector2 transformedCenter = TransformPoint(center);
        math::Vector2 scale = math::Vector2(mTransformMatrix.m[0][0], mTransformMatrix.m[1][1]);
        float scaledRadius = radius * std::max(scale.x, scale.y);
        
        // Normaliser les angles
        while (startAngle < 0) startAngle += math::TWO_PI;
        while (endAngle < 0) endAngle += math::TWO_PI;
        while (startAngle > math::TWO_PI) startAngle -= math::TWO_PI;
        while (endAngle > math::TWO_PI) endAngle -= math::TWO_PI;
        
        if (startAngle > endAngle) {
            std::swap(startAngle, endAngle);
        }
        
        const int segments = 64;
        std::vector<math::Vector2> points;
        
        // Générer les points de l'arc transformés
        for (int i = 0; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * i / segments;
            math::Vector2 point = {
                transformedCenter.x + scaledRadius * std::cos(angle),
                transformedCenter.y + scaledRadius * std::sin(angle)
            };
            points.push_back(point);
        }
        
        // Dessiner selon le mode
        switch (mode) {
            case 0: // Arc ouvert
                if (mUseStroke) {
                    for (size_t i = 0; i < points.size() - 1; i++) {
                        Line(points[i], points[i + 1]);
                    }
                }
                break;
                
            case 1: // Arc fermé
                if (mUseFill) {
                    points.push_back(transformedCenter);
                    Polygon(points, true);
                }
                if (mUseStroke) {
                    for (size_t i = 0; i < points.size() - 2; i++) {
                        Line(points[i], points[i + 1]);
                    }
                }
                break;
                
            case 2: // Forme de tarte
                if (mUseFill) {
                    std::vector<math::Vector2> piePoints;
                    piePoints.push_back(transformedCenter);
                    piePoints.insert(piePoints.end(), points.begin(), points.end());
                    Polygon(piePoints, true);
                }
                if (mUseStroke) {
                    for (size_t i = 0; i < points.size() - 1; i++) {
                        Line(points[i], points[i + 1]);
                    }
                    Line(transformedCenter, points.front());
                    Line(transformedCenter, points.back());
                }
                break;
        }
    }

    void Application::ThickArc(const math::Vector2& center, float radius,
                              float startAngle, float endAngle, float thickness) {
        if (radius <= 0 || thickness <= 0) return;
        
        // Normaliser les angles
        while (startAngle < 0) startAngle += math::TWO_PI;
        while (endAngle < 0) endAngle += math::TWO_PI;
        while (startAngle > math::TWO_PI) startAngle -= math::TWO_PI;
        while (endAngle > math::TWO_PI) endAngle -= math::TWO_PI;
        
        if (startAngle > endAngle) {
            std::swap(startAngle, endAngle);
        }
        
        const int segments = 64;
        std::vector<math::Vector2> innerPoints;
        std::vector<math::Vector2> outerPoints;
        
        // Générer les points intérieurs et extérieurs
        for (int i = 0; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * i / segments;
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            
            innerPoints.push_back({
                center.x + (radius - thickness/2) * cosA,
                center.y + (radius - thickness/2) * sinA
            });
            
            outerPoints.push_back({
                center.x + (radius + thickness/2) * cosA,
                center.y + (radius + thickness/2) * sinA
            });
        }
        
        // Dessiner l'arc épais
        if (mUseFill) {
            std::vector<math::Vector2> quadPoints;
            for (size_t i = 0; i < innerPoints.size(); i++) {
                quadPoints.push_back(innerPoints[i]);
            }
            for (size_t i = outerPoints.size(); i > 0; i--) {
                quadPoints.push_back(outerPoints[i-1]);
            }
            Polygon(quadPoints, true);
        }
        
        // Dessiner le contour
        if (mUseStroke && mStrokeWeight > 0) {
            for (size_t i = 0; i < innerPoints.size() - 1; i++) {
                if (mStrokeWeight > 1.0f) {
                    ThickLine(innerPoints[i], innerPoints[i + 1], mStrokeWeight);
                    ThickLine(outerPoints[i], outerPoints[i + 1], mStrokeWeight);
                } else {
                    Line(innerPoints[i], innerPoints[i + 1]);
                    Line(outerPoints[i], outerPoints[i + 1]);
                }
            }
            
            // Dessiner les côtés
            if (mStrokeWeight > 1.0f) {
                ThickLine(innerPoints.front(), outerPoints.front(), mStrokeWeight);
                ThickLine(innerPoints.back(), outerPoints.back(), mStrokeWeight);
            } else {
                Line(innerPoints.front(), outerPoints.front());
                Line(innerPoints.back(), outerPoints.back());
            }
        }
    }

    void Application::EllipticalArc(const math::Vector2& center, float radiusX, float radiusY,
                                   float startAngle, float endAngle, float rotation) {
        if (radiusX <= 0 || radiusY <= 0) return;
        
        // Normaliser les angles
        while (startAngle < 0) startAngle += math::TWO_PI;
        while (endAngle < 0) endAngle += math::TWO_PI;
        while (startAngle > math::TWO_PI) startAngle -= math::TWO_PI;
        while (endAngle > math::TWO_PI) endAngle -= math::TWO_PI;
        
        if (startAngle > endAngle) {
            std::swap(startAngle, endAngle);
        }
        
        const int segments = 64;
        std::vector<math::Vector2> points;
        float cosRot = std::cos(rotation);
        float sinRot = std::sin(rotation);
        
        // Générer les points de l'arc elliptique
        for (int i = 0; i <= segments; i++) {
            float angle = startAngle + (endAngle - startAngle) * i / segments;
            float cosA = std::cos(angle);
            float sinA = std::sin(angle);
            
            // Point non-rotaté
            math::Vector2 point = {
                center.x + radiusX * cosA,
                center.y + radiusY * sinA
            };
            
            // Appliquer la rotation
            if (rotation != 0.0f) {
                float dx = point.x - center.x;
                float dy = point.y - center.y;
                point.x = center.x + dx * cosRot - dy * sinRot;
                point.y = center.y + dx * sinRot + dy * cosRot;
            }
            
            points.push_back(point);
        }
        
        // Dessiner l'arc
        if (mUseFill) {
            points.push_back(center);
            Polygon(points, true);
        }
        
        if (mUseStroke) {
            for (size_t i = 0; i < points.size() - 1; i++) {
                if (mStrokeWeight > 1.0f) {
                    ThickLine(points[i], points[i + 1], mStrokeWeight);
                } else {
                    Line(points[i], points[i + 1]);
                }
            }
        }
    }

    void Application::Ring(const math::Vector2& center, float innerRadius, float outerRadius) {
        if (innerRadius <= 0 || outerRadius <= 0 || innerRadius >= outerRadius) return;
        
        const int segments = 64;
        std::vector<math::Vector2> points;
        
        // Générer les points de l'anneau
        for (int i = 0; i <= segments * 2; i++) {
            float angle = math::TWO_PI * i / segments;
            float radius = (i <= segments) ? outerRadius : innerRadius;
            if (i > segments) angle = math::TWO_PI * (2 * segments - i) / segments;
            
            math::Vector2 point = {
                center.x + radius * std::cos(angle),
                center.y + radius * std::sin(angle)
            };
            points.push_back(point);
        }
        
        // Dessiner l'anneau
        if (mUseFill) {
            Polygon(points, true);
        }
        
        if (mUseStroke) {
            for (size_t i = 0; i < points.size() - 1; i++) {
                if (mStrokeWeight > 1.0f) {
                    ThickLine(points[i], points[i + 1], mStrokeWeight);
                } else {
                    Line(points[i], points[i + 1]);
                }
            }
        }
    }

    void Application::Pie(const math::Vector2& center, float radius,
                         float startAngle, float endAngle) {
        // Utiliser la méthode Arc avec le mode tarte
        Arc(center, radius, startAngle, endAngle, 2);
    }

    // Fonction utilitaire pour le test de point dans polygone
    bool Application::IsPointInPolygon(const math::Vector2& point, const std::vector<math::Vector2>& vertices) {
        bool inside = false;
        size_t j = vertices.size() - 1;
        
        for (size_t i = 0; i < vertices.size(); i++) {
            if (((vertices[i].y > point.y) != (vertices[j].y > point.y)) &&
                (point.x < (vertices[j].x - vertices[i].x) * (point.y - vertices[i].y) / 
                (vertices[j].y - vertices[i].y) + vertices[i].x)) {
                inside = !inside;
            }
            j = i;
        }
        
        return inside;
    }

    // Modifier aussi la méthode Circle pour utiliser StrokeWeight
    void Application::Circle(const math::Vector2& center, float radius) {
        Ellipse(center, radius, radius);
    }

    math::Vector2 Application::GetMousePosition() const {
        return events::Mouse::GetPosition();
    }

    bool Application::IsKeyPressed(events::KeyCode key) const {
        return events::Keyboard::IsKeyPressed(key);
    }

    bool Application::IsMouseButtonPressed(events::MouseButton button) const {
        return events::Mouse::IsButtonPressed(button);
    }

    void Application::PushMatrix() {
        mTransformStack.push(mTransformMatrix);
    }

    void Application::PopMatrix() {
        if (!mTransformStack.empty()) {
            mTransformMatrix = mTransformStack.top();
            mTransformStack.pop();
        }
    }

    void Application::ResetMatrix() {
        mTransformMatrix = math::Matrix3x3::Identity();
    }

    void Application::Translate(float x, float y) {
        mTransformMatrix = mTransformMatrix * math::Matrix3x3::Translation(x, y);
    }

    void Application::Translate(const math::Vector2& translation) {
        Translate(translation.x, translation.y);
    }

    void Application::Rotate(float angle) {
        mTransformMatrix = mTransformMatrix * math::Matrix3x3::Rotation(angle);
    }

    void Application::RotateDegrees(float degrees) {
        Rotate(degrees * math::PI / 180.0f);
    }

    void Application::Scale(float sx, float sy) {
        mTransformMatrix = mTransformMatrix * math::Matrix3x3::Scaling(sx, sy);
    }

    void Application::Scale(float scale) {
        Scale(scale, scale);
    }

    void Application::Scale(const math::Vector2& scale) {
        Scale(scale.x, scale.y);
    }

    math::Matrix3x3 Application::GetCurrentMatrix() const {
        return mTransformMatrix;
    }

    math::Vector2 Application::TransformPoint(const math::Vector2& point) const {
        return mTransformMatrix.TransformPoint(point);
    }

} // namespace nkentseu