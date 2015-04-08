Pod::Spec.new do |s|
  s.name         = "GLPubSub"
  s.version      = "1.0.1"
  s.summary      = "A wrapper of NSNotificationCenter to make pub sub easier"
  s.homepage     = "https://github.com/Glow-Inc/GLPubSub"
  s.license      = "MIT"
  s.author       = "Glow, Inc."
  s.social_media_url   = "https://github.com/Glow-Inc"

  s.ios.deployment_target = "5.0"
  s.osx.deployment_target = "10.7"

  s.source       = { :git => "https://github.com/Glow-Inc/GLPubSub.git", :tag => s.version }
  s.source_files  = "GLPubSub/*.{h,m}"

  s.requires_arc = true
end
