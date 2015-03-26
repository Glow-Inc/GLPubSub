Pod::Spec.new do |s|
  s.name         = "GLPubSub"
  s.version      = "1.0.0"
  s.summary      = "A wrapper of NSNotificationCenter to make pub sub easier"
  s.homepage     = "https://github.com/Glow-Inc/GLPubSub"
  s.license      = "MIT"
  s.author       = "Glow, Inc."
  s.social_media_url   = "https://github.com/Glow-Inc"

  s.platform     = :ios, "5.0"
  s.source       = { :git => "https://github.com/Glow-Inc/GLPubSub.git", :tag => s.version }
  s.source_files  = "GLPubSub/*.{h,m}"

  s.requires_arc = true
end
