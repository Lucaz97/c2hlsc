from openai import OpenAI
import os
client = OpenAI(
api_key = os.environ.get("LLAMA_API_KEY"),
base_url = "https://api.llama-api.com"
)

response = client.chat.completions.create(
    model="llama3.1-70b",
    messages=[
        {"role": "system", "content": "You should only reply oooh for yes and uhm for no"},
        {"role": "user", "content": "is 2+2  =5?"}
    ],
    top_p=0.1,
    temperature=0.15
)

#print(response)
print(response.model_dump_json(indent=2))
print(response.choices[0].message.content)
