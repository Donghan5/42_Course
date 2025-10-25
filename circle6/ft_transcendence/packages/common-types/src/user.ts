export interface User {
  id: number;
  google_id: string;
  email: string;
  name: string;
  token: string;
  nickname: string | null;
  avatar_url: string | null;
  profile_setup_complete?: boolean;
}
