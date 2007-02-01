
#include <stdlib.h>

#include "agent.h"

/* format is:
 *   type/ip/port
 */
NiceCandidate *
nice_candidate_from_string (const gchar *s)
{
  NiceCandidateType type;
  NiceCandidate *candidate;
  guint32 ip;
  guint16 port;
  gchar **bits;

  if (s == NULL || s[0] == '\0')
    return NULL;

  bits = g_strsplit (s, "/", 3);

  if (g_strv_length (bits) != 3)
    return NULL;

  switch (bits[0][0])
    {
    case 'H':
      type = NICE_CANDIDATE_TYPE_HOST;
      break;
    case 'S':
      type = NICE_CANDIDATE_TYPE_SERVER_REFLEXIVE;
      break;
    case 'P':
      type = NICE_CANDIDATE_TYPE_PEER_REFLEXIVE;
      break;
    case 'R':
      type = NICE_CANDIDATE_TYPE_RELAYED;
      break;
    default:
      return NULL;
    }

  /* extract IP address */

  if (inet_pton (AF_INET, bits[1], &ip) < 1)
    return NULL;

  /* extract port */

  port = strtol (bits[2], NULL, 10);

  candidate = nice_candidate_new (type);
  nice_address_set_ipv4 (&candidate->addr, ntohl (ip));
  candidate->port = port;

  return candidate;
}

gchar *
nice_candidate_to_string (NiceCandidate *candidate)
{
  gchar *addr_tmp;
  gchar *ret;
  gchar type;

  switch (candidate->type)
    {
    case NICE_CANDIDATE_TYPE_HOST:
      type = 'H';
      break;
    case NICE_CANDIDATE_TYPE_SERVER_REFLEXIVE:
      type = 'S';
      break;
    case NICE_CANDIDATE_TYPE_PEER_REFLEXIVE:
      type = 'P';
      break;
    case NICE_CANDIDATE_TYPE_RELAYED:
      type = 'R';
      break;
    default:
      return NULL;
    }

  addr_tmp = nice_address_to_string (&(candidate->addr));
  ret = g_strdup_printf ("%c/%s/%d", type, addr_tmp, candidate->port);
  g_free (addr_tmp);
  return ret;
}

