#version 460 core

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec2 UV;
} fs;

out vec4 FragColor;

// === Object inputs ===
uniform vec4  objectColor      = vec4(1.0);
uniform bool  bUseTexture      = false;
uniform sampler2D objectTexture;

// === Camera ===
uniform vec3 viewPos;

// === Simple Phong material ===
struct Material {
    vec3  ambientColor;
    float ambientStrength;
    vec3  diffuseColor;
    vec3  specularColor;
    float shininess;        // exponent (e.g., 16..128)
};
uniform Material material;

// === Directional lights (key + fill) ===
struct DirLight {
    vec3 direction;   // pointing *from* light *to* scene (so usually (-) toward normal)
    vec3 color;
    float intensity;  // 0..1+
};
uniform DirLight dirLight;   // key
uniform DirLight fillLight;  // fill

vec3 applyDirLight(DirLight L, vec3 base, vec3 N, vec3 V)
{
    vec3 Ldir = normalize(-L.direction);
    float diff = max(dot(N, Ldir), 0.0);

    vec3 R = reflect(-Ldir, N);
    float spec = pow(max(dot(V, R), 0.0), max(material.shininess, 1.0));

    vec3 ambient  = material.ambientColor  * material.ambientStrength;
    vec3 diffuse  = material.diffuseColor  * diff;
    vec3 specular = material.specularColor * spec;

    return (ambient + (diffuse + specular) * L.intensity) * L.color * base;
}

void main()
{
    vec3 base = bUseTexture ? texture(objectTexture, fs.UV).rgb
                            : objectColor.rgb;

    vec3 N = normalize(fs.Normal);
    vec3 V = normalize(viewPos - fs.FragPos);

    // Sum two lights; keep a tiny floor so nothing is pitch black
    vec3 color = vec3(0.02); // tiny ambient floor
    color += applyDirLight(dirLight,  base, N, V);
    color += applyDirLight(fillLight, base, N, V);

    FragColor = vec4(color, 1.0);
}
